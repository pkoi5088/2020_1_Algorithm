#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class menu;
class fuel;
class kitchen;
class table;

class menu {
private:
	int max = -1;
	int time = -1;

public:
	int number = 0;
	menu() {}

	void setMax(int N) { max = N; }
	int getMax() { return max; }
	void setTime(int N) { time = N; }
	int getTime() { return time; }
	void setNum(int N) { number = N; }

	bool operator> (const menu& m) {
		return this->time < m.time;
	}

	bool operator< (const menu& m) {
		return this->time > m.time;
	}

	bool operator>= (const menu& m) {
		return this->time <= m.time;
	}

	bool operator<= (const menu& m) {
		return this->time >= m.time;
	}

	bool operator== (const menu& m) {
		return this->number == m.number;
	}
};

class fuel {
private:
	vector<pair<menu, vector<int>>> order;
	int time = 0;

public:
	int getTime() { return time; }
	int getFastTiming(menu m) { //화구에 대해서 들어갈 수 있는 가장 빠른 시간반환
		int t = 0;
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax()) {
				return t;
			}
			t += order[i].first.getTime();
		}
		return t;
	}

	int getAroundTiming(menu m, int ftime, int& index) { //화구에 대해서 ftime에 가까운 시간차이를 반환
		int t = time, lasttime = time + m.getTime(), i;
		for (i = order.size() - 1; i >= 0; i--) {
			if (m == order[i].first&&order[i].second.size() < order[i].first.getMax()) {
				break;
			}
			t -= order[i].first.getTime();
		}
		int a = abs(t - ftime), b = abs(lasttime - ftime);
		if (a > b || t == 0) {
			index = order.size();
			return b;
		}
		index = i;
		return a;
	}

	void insertBack(menu m, int tn) {
		//맨뒤에 추가한다
		pair<menu, vector<int>> p;
		p.first = m; p.second.push_back(tn);
		order.push_back(p);
		time += m.getTime();
	}

	void insertFast(menu m, int tn) {
		//메뉴 m을 가장 빠른자리에 넣는다.
		int i;
		for (i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax())
				break;
		}
		if (i == order.size())
			insertBack(m, tn);
		else {
			order[i].second.push_back(tn);
			time += m.getTime();
		}
	}

	void insertAround(menu m, int tn, int index) {
		//메뉴 m을 time과 가까운 가장빠른시간자리에 넣는다.
		if (index >= order.size()) {
			insertBack(m, tn);
		}
		else {
			order[index].second.push_back(tn);
			time += m.getTime();
		}
	}

	bool isIn(menu m) {
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first)
				return true;
		}
		return false;
	}

	int getSize() {
		return order.size();
	}

	pair<menu, vector<int>> popOrder() {
		pair<menu, vector<int>> r = order[0];
		order.erase(order.begin());
		return r;
	}

	void print() {
		for (int i = 0; i < order.size(); i++) {
			cout << "{" << order[i].first.number << ", ";
			for (int j = 0; j < order[i].second.size(); j++) {
				cout << order[i].second.at(j) << ' ';
			}
			cout << "}\n";
		}
	}
};

class table {
private:
	int num = 0;
	int wait_time = 0;
	int N;
	int wait = 0;

public:
	void setNum(int N) { num = this->N = N; }
	void addTime() { if (wait) wait_time++; }
	void serving() { num--;	wait = num; }
	int getWait() { return wait_time; }
};

class kitchen {
private:
	fuel *arr;
	int N;

public:
	kitchen(int N) { arr = new fuel[N]; this->N = N; }

	int getFastFuel(menu m) {
		//메뉴 m이 들어갈 수 있는 자리 중 가장 빠른 화구번호를 반환
		int r = 0, t = arr[0].getFastTiming(m);
		for (int i = 1; i < N; i++) {
			int ct = arr[i].getFastTiming(m);
			if (t > ct) {
				r = i; t = ct;
			}
		}
		return r;
	}

	int getAroundFuel(menu m, int ftime, int&index) {
		//메뉴 m이 들어갈 수 있는 자리 중 ftime에 가까운 자리가 있는 화구 번호
		int r = 0, t = arr[0].getAroundTiming(m, ftime, index);
		for (int i = 1; i < N; i++) {
			int tmp;
			int ct = arr[i].getAroundTiming(m, ftime, tmp);
			if (t > ct) {
				r = i; t = ct; index = tmp;
			}
		}
		return r;
	}

	void insertFast(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		for (int i = 0; i < order.size(); i++) {
			arr[getFastFuel(order[i])].insertFast(order[i], tn);
		}
	}

	void insertBrute(vector<menu> order, int tn) {
		//incase 메뉴를 몇번화구에 넣을건지
	}

	void insertGreedy(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		//가장 시간이 오래걸리는 메뉴를 가장 최선으로 넣고 조리가 끝나는 시간(ftime)을 구한다.
		menu firstIn = order[0]; int flag = 1;
		for (int i = 0; i < order.size(); i++) {
			if (isIn(order[i]) == false) {
				firstIn = order[i];
				order.erase(order.begin() + i);
				flag = 0;
				break;
			}
		}
		if (flag)
			order.erase(order.begin());
		int findex = getFastFuel(firstIn), ftime = arr[findex].getFastTiming(firstIn) + firstIn.getTime();
		arr[findex].insertFast(firstIn, tn);
		for (int i = 0; i < order.size(); i++) {
			int index = -1;
			arr[getAroundFuel(order[i], ftime, index)].insertAround(order[i], tn, index);
		}
	}

	bool compare(menu& a, menu& b) {
		if (a.getTime() == b.getTime()) {
			return isIn(a);
		}
		else {
			return a.getTime() > b.getTime();
		}
	}

	bool isIn(menu a) {
		for (int i = 0; i < N; i++) {
			if (arr[i].isIn(a))
				return true;
		}
		return false;
	}

	vector<pair<int, int>> execute(int T) {
		//first : 음식이 처음 나올 때, second ; 마지막음식이 나올 때
		vector<pair<int, int>> r;
		for (int i = 0; i < T; i++)
			r.push_back({ -1,-1 });

		for (int i = 0; i < N; i++) {
			int time = 0;//현재시간
			while (arr[i].getSize()) {
				pair<menu, vector<int>> t = arr[i].popOrder();
				vector<int> v = t.second;
				time += t.first.getTime();
				for (int k = 0; k < v.size(); k++) {
					int tn = v[k];//테이블 번호
					r[tn].second = max(r[tn].second, time);
					if (r[tn].first == -1)
						r[tn].first = time;
					else
						r[tn].first = min(r[tn].first, time);
				}
			}
		}
		return r;
	}

	void print() {
		for (int i = 0; i < N; i++) {
			arr[i].print();
			cout << endl;
		}
	}
};

int main() {
	int N, M, T;
	cout << "화구의 개수 : ";
	cin >> N;
	cout << "메뉴의 개수 : ";
	cin >> M;

	menu *board = new menu[M];
	kitchen kitchen0(N), kitchen1(N), kitchen2(N);
	for (int i = 0; i < M; i++) {
		int j;
		cout << "메뉴 " << i << "의 최대조리 개수와 시간 : ";
		cin >> j;
		board[i].setMax(j);
		cin >> j;
		board[i].setTime(j);
		board[i].setNum(i);
	}

	cout << "테이블의 개수 : ";
	cin >> T;
	table *t = new table[T];
	for (int i = 0; i < T; i++) {
		int j;
		cout << i << "번 테이블 메뉴의 개수 : ";
		cin >> j;
		t[i].setNum(j);
		vector<menu> order;
		while (j--) {
			int number;
			cin >> number;
			order.push_back(board[number]);
		}
		kitchen0.insertFast(order, i);
		//kitchen1.insertBrute(order, i);
		kitchen2.insertGreedy(order, i);
	}
	cout << "-------------------------------\n";
	//kitchen2.print();
	vector<pair<int, int>> r0 = kitchen0.execute(T);
	for (int i = 0; i < r0.size(); i++) {
		cout << r0[i].second << ' ' << r0[i].first << ' ';
		cout << r0[i].second - r0[i].first << endl;
	}
	cout << "-------------------------------\n";

	cout << "-------------------------------\n";
	vector<pair<int, int>> r2 = kitchen2.execute(T);
	for (int i = 0; i < r2.size(); i++) {
		cout << r2[i].second << ' ' << r2[i].first << ' ';
		cout << r2[i].second - r2[i].first << endl;
	}
	return 0;
}
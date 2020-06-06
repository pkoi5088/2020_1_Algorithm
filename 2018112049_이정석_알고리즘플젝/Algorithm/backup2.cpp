#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <ctime>

using namespace std;

class menu;
class fuel;
class kitchen;

void Permutation(int idx, int st, vector<menu>& op, vector<int>& eq, vector<int>& sel, vector<vector<menu>>& result);

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

	bool operator!= (const menu& m) {
		return this->number != m.number;
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

	int getAroundTiming(menu m, int under, int over, int& index) {
		int t = time, lasttime = time + m.getTime(), i;
		for (i = order.size() - 1; i >= 0; i--) {
			if (m == order[i].first&&order[i].second.size() < order[i].first.getMax()) {
				break;
			}
			t -= order[i].first.getTime();
		}
		//t=0일때
		if (t == 0) {
			index = order.size();
			return lasttime;
		}
		else if (t < under) {
			index = i;
			return t;
		}
		else if (t > over) {
			int t2 = 0;
			int j;
			for (j = 0; j < order.size(); j++) {
				t += order[j].first.getTime();
				if (m == order[j].first&&order[j].second.size() < order[j].first.getMax()) {
					break;
				}
			}
			if (t == t2) {
				index = i;
				return t;
			}
			else {
				if (abs(t2 - under) > abs(t - over)) {
					index = i;
					return t;
				}
				else {
					index = j;
					return t2;
				}
			}
		}
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
		}
	}

	void insertAround(menu m, int tn, int index) {
		//메뉴 m을 time과 가까운 가장빠른시간자리에 넣는다.
		if (index >= order.size()) {
			insertBack(m, tn);
		}
		else {
			order[index].second.push_back(tn);
		}
	}

	void insertSimul(menu m, int tn, int index) {
		//메뉴 m을 time과 가까운 가장빠른시간자리에 넣는다.
		order[index].second.push_back(tn);
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

	vector<int> getAble(menu m) {
		vector<int> r;
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax())
				r.push_back(i);
		}
		if (order.size() == 0 || !(m == order.back().first&&order.back().second.size() != order.back().first.getMax()))
			r.push_back(-1);
		return r;
	}

	bool isIn(menu m, int under, int over, int& index) {
		int t = 0;
		for (int i = 0; i < order.size(); i++) {
			t += order[i].first.getTime();
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax()) {
				if (under <= t && t <= over) {
					index = i;
					return true;
				}
			}
		}
		return false;
	}

	int getUnder(int tn) {
		int t = 0;
		for (int i = 0; i < order.size(); i++) {
			t += order[i].first.getTime();
			for (int j = 0; j < order[i].second.size(); j++) {
				if (order[i].second.at(j) == tn) {
					return t;
				}
			}
		}
		return -1;
	}

	int getOver(int tn) {
		int t = 0, r = -1;
		for (int i = 0; i < order.size(); i++) {
			t += order[i].first.getTime();
			for (int j = 0; j < order[i].second.size(); j++) {
				if (order[i].second.at(j) == tn) {
					r = t; break;
				}
			}
		}
		return r;
	}
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

	void insertFast(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		for (int i = 0; i < order.size(); i++) {
			arr[getFastFuel(order[i])].insertFast(order[i], tn);
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	void insertGreedy(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		//가장 시간이 오래걸리는 메뉴를 가장 최선으로 넣고 조리가 끝나는 시간(ftime)을 구한다.
		menu firstIn = order[0];
		int findex = getFastFuel(firstIn), ftime = arr[findex].getFastTiming(firstIn) + firstIn.getTime();
		arr[findex].insertFast(firstIn, tn);
		//하한설정, 상한설정 -1일수 없다.
		int under = ftime, over = ftime;
		for (int i = 1; i < order.size(); i++) {
			int index = -1;
			arr[getAroundFuel(order[i], under, over, index)].insertAround(order[i], tn, index);
			under = getUnder(tn); over = getOver(tn);
		}
	}

	int getAroundFuel(menu m, int under, int over, int& index) {
		/*
		1. under <= t <= over를 만족하는 자리가 있는 화구번호
		2. 만약 다 만족하지 않으면 over-under가 가장 작은 화구번호
		*/
		vector<pair<int, int>> r;//first 화구번호 second index
		for (int i = 0; i < N; i++) {
			int in = -1;
			if (arr[i].isIn(m, under, over, in)) {
				r.push_back({ i,in });
			}
		}
		if (r.size() > 0) {
			index = r[0].second;
			return r[0].first;
		}
		//over or under를 갱신해야 하는 상황
		for (int i = 0; i < N; i++) {
			int in = -1;
			//z : 만약 i 화구에 넣게 된다면 최적의 시간을 반환
			int z = arr[i].getAroundTiming(m, under, over, in);
			int m1 = min(z, under), m2 = max(z, over);
			r.push_back({ abs(m1 - m2),in });
		}

		int oi = 0, z = r[0].first;
		for (int i = 1; i < r.size(); i++) {
			if (r[i].first < z) {
				oi = i;
				z = r[i].first;
			}
		}
		index = r[oi].second;
		return oi;
	}

	int getUnder(int tn) {
		int t = -1;
		for (int i = 0; i < N; i++) {
			int z = arr[i].getUnder(tn);
			if ((t == -1 && z != -1) || (t != -1 && z != -1 && z < t)) {
				t = z;
			}
		}
		return t;
	}

	int getOver(int tn) {
		int t = -1;
		for (int i = 0; i < N; i++) {
			int z = arr[i].getOver(tn);
			if ((t == -1 && z != -1) || (t != -1 && z != -1 && z > t)) {
				t = z;
			}
		}
		return t;
	}
	//------------------------------------------------------------------------------------------------------------------
	void insertBrute(vector<menu> order, int tn) {
		//vector<vector<menu>> all;
		//all.push_back(order);
		//result int 최소화 시간
		vector<pair<int, vector<fuel>>> result;
		//{
		//	vector<menu> op; vector<int> eq;
		//	vector<int> sel(order.size(), -1);
		//	op.push_back(order[0]); eq.push_back(1);
		//	for (int i = 1; i < order.size(); i++) {
		//		if (order[i] == op.back()) {
		//			eq.back() += 1;
		//		}
		//		else {
		//			op.push_back(order[i]);
		//			eq.push_back(1);
		//		}
		//	}
		//	Permutation(0, 0, op, eq, sel, all);
		//}
		getBruteTime(order, tn, result);

		//갱신작업
		int mini = result[0].first;
		int under, over, miniIndex = 0;
		getMidTime(under, over, result[0].second, tn);
		for (int i = 1; i < result.size(); i++) {
			if (result[i].first < mini) {
				mini = result[i].first;
				getMidTime(under, over, result[i].second, tn);
				miniIndex = i;
			}
			if (result[i].first == mini) {
				int a, b;
				getMidTime(a, b, result[i].second, tn);
				if (abs(over - under) > abs(b - a)) {
					under = a; over = b;
					miniIndex = i;
				}
				else if (abs(over - under) == abs(b - a)) {
					if (over > b) {
						under = a; over = b;
						miniIndex = i;
					}
				}
			}
		}

		for (int i = 0; i < result[miniIndex].second.size(); i++) {
			arr[i] = result[miniIndex].second.at(i);
		}
	}

	void getBruteTime(vector<menu> order, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		//order 넣을 메뉴 순서
		vector<int> fnum(order.size(), 0);
		vector<pair<int, vector<fuel>>> result;
		for (int i = 0;; i++) {
			getAbleTime(order, fnum, tn, result);
			fnum[fnum.size() - 1]++;
			for (int j = fnum.size() - 1; j > 0; j--) {
				fnum[j - 1] += fnum[j] / N;
				fnum[j] = fnum[j] % N;
			}
			if (fnum[0] >= N)
				break;
			//for (int j = 0; j <fnum.size(); j++) {
			//	cout << fnum[j] << ' ';
			//}
			//cout << endl;
		}
		//cout << result.size() << endl;
		//sort(result.begin(), result.end());
		int mini = result[0].first;
		for (int i = 1; i < result.size(); i++) {
			if (result[i].first < mini) {
				mini = result[i].first;
			}
		}
		for (int i = 0; i < result.size(); i++) {
			if (mini == result[i].first) {
				result_.push_back(result[i]);
			}
		}
	}

	void getAbleTime(vector<menu> order, vector<int> fnum, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		vector<fuel> sboard;
		for (int i = 0; i < N; i++)
			sboard.push_back(arr[i]);
		vector<vector<fuel>> result;
		int ans = order.size();
		subAble(sboard, order, fnum, tn, 0, ans, result);
		int index = 0, mini = execute(result[0], tn);
		for (int i = 1; i < result.size(); i++) {
			int r = execute(result[i], tn);
			if (r < mini) {
				index = i; mini = r;
			}
		}
		result_.push_back({ mini,result[index] });
	}

	void subAble(vector<fuel>simul, vector<menu>&order, vector<int>&num, int& tn, int dep, int& ans, vector<vector<fuel>>& result) {
		if (dep == ans) {
			result.push_back(simul);
		}
		else {
			menu m = order[dep]; int fnum = num[dep]; vector<int> index = simul[fnum].getAble(m);
			for (int i = 0; i < index.size(); i++) {
				vector<fuel> s = simul;
				if (index[i] == -1) {
					s[fnum].insertBack(m, tn);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
				else {
					s[fnum].insertSimul(m, tn, index[i]);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	bool isIn(menu a) {
		for (int i = 0; i < N; i++) {
			if (arr[i].isIn(a))
				return true;
		}
		return false;
	}

	int execute(vector<fuel> s, int tn_) {
		//first : 음식이 처음 나올 때, second ; 마지막음식이 나올 때
		int first = -1, last = -1;

		for (int i = 0; i < N; i++) {
			int time = 0;//현재시간
			while (s[i].getSize()) {
				pair<menu, vector<int>> t = s[i].popOrder();
				vector<int> v = t.second;
				time += t.first.getTime();
				for (int k = 0; k < v.size(); k++) {
					int tn = v[k];//테이블 번호
					if (tn_ == tn) {
						if (first == -1)
							first = time;
						else
							first = min(time, first);
						if (last == -1)
							last = time;
						else
							last = max(time, last);
					}
				}
			}
		}
		return abs(first - last);
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
			cout << i << endl;
			arr[i].print();
			cout << endl;
		}
	}

	void getMidTime(int& under, int& over, vector<fuel> f, int tn) {
		int t = -1;
		for (int i = 0; i < f.size(); i++) {
			int z = f[i].getUnder(tn);
			if ((t == -1 && z != -1) || (t != -1 && z != -1 && z < t)) {
				t = z;
			}
		}
		under = t;

		t = -1;
		for (int i = 0; i < f.size(); i++) {
			int z = f[i].getOver(tn);
			if ((t == -1 && z != -1) || (t != -1 && z != -1 && z > t)) {
				t = z;
			}
		}
		over = t;
	}
};

/* 순열 알고리즘 */
void Permutation(int idx, int st, vector<menu>& op, vector<int>& eq, vector<int>& sel, vector<vector<menu>>& result) {
	if (idx == op.size()) {
		vector<menu> v;
		for (int j = 0; j < sel.size(); j++) {
			v.push_back(op[sel[j]]);
		}
		result.push_back(v);
		return;
	}
	if (!eq[idx])
		Permutation(idx + 1, 0, op, eq, sel, result);
	else {
		eq[idx] -= 1;
		for (int s = st; s < sel.size(); s++) {
			if (sel[s] < 0) {
				sel[s] = idx;
				Permutation(idx, s + 1, op, eq, sel, result);
				sel[s] = -1;
			}
		}
		eq[idx] += 1;
	}
}

int main() {
	int N, M, T;
	clock_t start, end;
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
	for (int i = 0; i < T; i++) {
		int j;
		cout << i << "번 테이블 메뉴의 개수 : ";
		cin >> j;
		vector<menu> order;
		while (j--) {
			int number;
			cin >> number;
			order.push_back(board[number]);
		}
		start = clock();
		kitchen0.insertFast(order, i);
		end = clock();
		cout << "insertFast : " << (double)(end - start) << "(ms)\n";
		start = clock();
		kitchen1.insertBrute(order, i);
		end = clock();
		cout << "insertBrute : " << (double)(end - start) << "(ms)\n";
		start = clock();
		kitchen2.insertGreedy(order, i);
		end = clock();
		cout << "insertGreedy : " << (double)(end - start) << "(ms)\n";
		start = clock();
	}
	cout << "-------------------------------\n";
	vector<pair<int, int>> r0 = kitchen0.execute(T);
	for (int i = 0; i < r0.size(); i++) {
		cout << r0[i].second << ' ' << r0[i].first << ' ';
		cout << r0[i].second - r0[i].first << endl;
	}
	cout << "-------------------------------\n";
	//kitchen1.print();
	vector<pair<int, int>> r1 = kitchen1.execute(T);
	for (int i = 0; i < r1.size(); i++) {
		cout << r1[i].second << ' ' << r1[i].first << ' ';
		cout << r1[i].second - r1[i].first << endl;
	}
	cout << "-------------------------------\n";
	//kitchen2.print();
	vector<pair<int, int>> r2 = kitchen2.execute(T);
	for (int i = 0; i < r2.size(); i++) {
		cout << r2[i].second << ' ' << r2[i].first << ' ';
		cout << r2[i].second - r2[i].first << endl;
	}
	return 0;
}
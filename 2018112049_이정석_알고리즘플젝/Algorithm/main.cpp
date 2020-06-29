#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <ctime>
#include <Windows.h>
#include <string>

using namespace std;

class menu;
class fuel;
class kitchen;

//�޴� Ŭ����
class menu {
private:
	//�ִ� ���� ������ �ɸ��� �ð�
	int max = -1;
	int time = -1;

public:
	//���� �ĺ� ��ȣ
	string name;
	int number = 0;
	menu() {}

	void setMax(int N) { max = N; }
	int getMax() { return max; }
	void setTime(int N) { time = N; }
	int getTime() { return time; }
	void setNum(int N) { number = N; }
	void setName(string N) { name = N; }

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

//ȭ�� Ŭ����
class fuel {
private:
	//ȭ���� �ִ� ���ĵ� first : �޴� second : ���̺��ȣvector
	vector<pair<menu, vector<int>>> order;
	int time = 0;

public:
	int getTime() { return time; }

	int getFastTiming(menu m) { //ȭ���� ���ؼ� �� �� �ִ� ���� ���� �ð���ȯ
		int t = 0;
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax()) {
				return t;
			}
			t += order[i].first.getTime();
		}
		return t;
	}

	//under over�� ���ؼ� menu�� ���� �� �ִ� ������ Ÿ�̹��� ��ȯ
	int getAroundTiming(menu m, int under, int over, int& index) {
		int t = time, lasttime = time + m.getTime(), i;
		//���������� �� �� �ִ� indexŽ��
		for (i = order.size() - 1; i >= 0; i--) {
			if (m == order[i].first&&order[i].second.size() < order[i].first.getMax()) {
				break;
			}
			t -= order[i].first.getTime();
		}
		//t=0�϶� = ȭ���� �޴��� �������� �ʴ´�.
		if (t == 0) {
			index = order.size();
			return lasttime;
		}//�ֱ� ������ under�����̴�. over�� ���ٴ� �ǹ�
		else if (t < under) {
			index = i;
			return t;
		}//over�����϶�
		else if (t > over) {
			int t2 = 0;
			int j;
			//under������ �����ϴ��� ��
			for (j = 0; j < order.size(); j++) {
				t += order[j].first.getTime();
				if (m == order[j].first&&order[j].second.size() < order[j].first.getMax()) {
					break;
				}
			}
			//under������ �������� ������
			if (t == t2) {
				index = i;
				return t;
			}
			else {//under������ ������ �� ��ȭ�ϴ� ������ ���� ���� ����
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

	//�ǵڿ� �߰��Ѵ�
	void insertBack(menu m, int tn) {
		pair<menu, vector<int>> p;
		p.first = m; p.second.push_back(tn);
		order.push_back(p);
		time += m.getTime();
	}

	//�޴� m�� ���� �����ڸ��� �ִ´�.
	void insertFast(menu m, int tn) {
		int i;
		//�տ������� �� �� �ִ� �ڸ��� Ž��
		for (i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax())
				break;
		}
		//������ insertBack
		if (i == order.size())
			insertBack(m, tn);
		else {//������ ���̺� ��ȣ �߰�
			order[i].second.push_back(tn);
		}
	}

	//�޴� m�� index�� �ִ´�.
	void insertAround(menu m, int tn, int index) {
		//index�� �ǵ� �κ��϶� insertBack
		if (index >= order.size()) {
			insertBack(m, tn);
		}
		else {//�ǵ� �κ��� �ƴϸ� �ش� index�� ��ȣ�߰�
			order[index].second.push_back(tn);
		}
	}

	//�޴� m�� index�� �ִ´�.(�̹� ����ó�� �� ����)
	void insertSimul(menu m, int tn, int index) {
		order[index].second.push_back(tn);
	}

	//ȭ���� menu�� �ִ°�?
	bool isIn(menu m) {
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first)
				return true;
		}
		return false;
	}

	//orderũ�� ��ȯ
	int getSize() {
		return order.size();
	}

	//�� �տ� �ִ� ���� ���¸� ��ȯ
	pair<menu, vector<int>> popOrder() {
		pair<menu, vector<int>> r = order[0];
		order.erase(order.begin());
		return r;
	}

	//�޴� m�� �� �� �ִ� index��ȣ�� ��ȯ �ǵ��� ��� -1
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

	//unter <= t <= over�� �����ϴ� �޴�m�� �ִ°�?
	int isIn(menu m, int under, int over, int& index) {
		int t = 0;
		for (int i = 0; i < order.size(); i++) {
			t += order[i].first.getTime();
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax()) {
				if (under <= t && t <= over) {
					index = i;
					return t;
				}
			}
		}
		return -1;
	}

	//tableNumber�� ���Ѽ� ��ȯ
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

	//tableNumber�� ���Ѽ� ��ȯ
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

	void print() {
		cout << "| ";
		for (int i = 0; i < order.size(); i++) {
			cout << order[i].first.name << ", { ";
			for (int j = 0; j < order[i].second.size(); j++) {
				cout << order[i].second.at(j) << ' ';
			}
			cout << "} | ";
		}
		cout << endl;
	}
};

class kitchen {
private:
	//arr�� ȭ�� �迭, N�� ȭ���� ����
	fuel *arr;
	int N;

public:
	kitchen(int N) { arr = new fuel[N]; this->N = N; }
	kitchen(kitchen& k) {//kitchen�� �������� ������
		N = k.N;
		arr = new fuel[N];
		for (int i = 0; i < N; i++) {
			arr[i] = k.arr[i];
		}
	}

	//�޴� m�� �� �� �ִ� �ڸ� �� ���� ���� ȭ����ȣ�� ��ȯ
	int getFastFuel(menu m) {
		//0���� N-1������ ȭ������ ���� Ÿ�̹��� �����س�����.
		int r = 0, t = arr[0].getFastTiming(m);
		for (int i = 1; i < N; i++) {
			int ct = arr[i].getFastTiming(m);
			if (t > ct) {
				r = i; t = ct;
			}
		}
		return r;
	}

	//tn���� �ֹ��� �޴����� ���� ���� ��ġ�� �ϳ��� �ִ´�.
	void insertFast(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		for (int i = 0; i < order.size(); i++) {
			arr[getFastFuel(order[i])].insertFast(order[i], tn);
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	void insertGreedy(vector<menu> order, int tn) {
		//1. ���� ȭ���� ���� �޴� 2. �����ɸ��� �޴� ������
		sort(order.begin(), order.end());

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
		//ȭ���� �ְ� �Ϸ�ð��� ��������
		int findex = getFastFuel(firstIn), ftime = arr[findex].getFastTiming(firstIn) + firstIn.getTime();
		arr[findex].insertFast(firstIn, tn);
		//�ʱ� under, over�� �����Ѵ�.
		int under = ftime, over = ftime;
		for (int i = 0; i < order.size(); i++) {
			//�ϳ��� ȭ���� ���� �� under�� over�� �����Ѵ�.
			int index = -1;
			arr[getAroundFuel(order[i], under, over, index)].insertAround(order[i], tn, index);
			under = getUnder(tn); over = getOver(tn);
		}
	}

	//������ �����ϴ� ȭ����ȣ�� ��ȯ
	int getAroundFuel(menu m, int under, int over, int& index) {
		/* ����
		1. under <= t <= over�� �����ϴ� �ڸ��� �ִ� ȭ����ȣ
		2. ���� �� �������� ������ over-under�� ���� ���� ȭ����ȣ
		*/
		vector<pair<int,pair<int, int>>> r;//first : t, second.first : ȭ����ȣ, second.second : index
		for (int i = 0; i < N; i++) {
			//under <= t <= over�� �����ϴ� ȭ���� ������
			int in = -1;
			int t = arr[i].isIn(m, under, over, in);
			if (t != -1) {
				//return�� �߰�
				r.push_back({ t ,{ i,in } });
			}
		}
		//����� �����Ѵ�
		
		if (r.size() > 0) {
			int a = r[0].first;
			for (int i = 1; i < r.size(); i++) {
				if (r[i].first < a)
					a = r[i].first;
			}
			for (int i = 0; i < r.size(); i++) {
				if (r[i].first == a) {
					index = r[i].second.second;
					return r[i].second.first;
				}
			}
		}
		//����� ���� over or under�� �����ؾ� �ϴ� ��Ȳ
		vector<pair<pair<int, int>, pair<int, int>>> sr;
		for (int i = 0; i < N; i++) {
			int in = -1;
			//z : ���� i ȭ���� �ְ� �ȴٸ� ������ �ð��� ��ȯ
			int z = arr[i].getAroundTiming(m, under, over, in);
			//under�� over�� ��Ұ���� ��Ȯ�ϹǷ� m2�� ���ο� over m1�� ���ο� under
			int m1 = min(z, under), m2 = max(z, over);
			//r�� ��Ȱ�� first.first : over-under �ð����� , first.second : index, vectorIndex = ȭ��index
			//r�� ��Ȱ�� second.first : under , second.second : over
			sr.push_back({ { abs(m1 - m2),in },{m1,m2} });
		}

		//�ð� ������ ���� ª�� ��츦 Ž��
		int oi = 0, z = sr[0].first.first, o = sr[0].second.second;
		for (int i = 1; i < sr.size(); i++) {
			if (sr[i].first.first < z) {
				oi = i;
				z = sr[i].first.first;
				o = sr[i].second.second;
			}
			else if (sr[i].first.first == z) {
				if (o > sr[i].second.second) {
					oi = i;
					z = sr[i].first.first;
					o = sr[i].second.second;
				}
			}
		}
		index = sr[oi].first.second;
		return oi;
	}

	//��ü ȭ���鿡 ���� under��ȯ
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

	//��ü ȭ���鿡 ���� over��ȯ
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
		//result first : �ð� ����, second : �� ���� ȭ������
		vector<pair<int, vector<fuel>>> result;
		//result���� order�� ���� fuel�� ��� ����� ���� �׿� ���� over-under�� ����ִ�.
		getBruteTime(order, tn, result);
		//�������� �ð� ���ݰ� �� ���� ��Ȳ�� �ִ�.
		//���͸�
		int mini = result[0].first;
		int under, over, miniIndex = 0;
		//ó�� ��Ȳ���� under�� over�� ���Ѵ�.
		getMidTime(under, over, result[0].second, tn);
		for (int i = 1; i < result.size(); i++) {
			//�ð� ������ �� ª�� ��� ����
			if (result[i].first < mini) {
				mini = result[i].first;
				getMidTime(under, over, result[i].second, tn);
				miniIndex = i;
			}else if (result[i].first == mini) {
				//�ð� ������ ������ over-under�� �ٸ� �� ����
				int a, b;
				getMidTime(a, b, result[i].second, tn);
				//�� ª�� ��츦 ã�� ��� ����
				if (abs(over - under) > abs(b - a)) {
					under = a; over = b;
					miniIndex = i;
				}
				else if (abs(over - under) == abs(b - a)) {
					//over-under���� ������ over�� ª�� ���� ����
					if (over > b) {
						under = a; over = b;
						miniIndex = i;
					}
				}
			}
		}

		//���������� ȭ������ ����
		for (int i = 0; i < N; i++) {
			arr[i] = result[miniIndex].second.at(i);
		}
	}

	//result_�� ��� ����� ���� ����
	void getBruteTime(vector<menu> order, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		//order ���� �޴� ����
		vector<int> fnum(order.size(), 0);
		vector<pair<int, vector<fuel>>> result;
		//���� �ݺ�
		for (int i = 0;; i++) {
			//���� fnum���¿��� ��� ����� ���� �߰�
			getAbleTime(order, fnum, tn, result);
			//���� fnum���·� ��ȯ
			fnum[fnum.size() - 1]++;
			for (int j = fnum.size() - 1; j > 0; j--) {
				fnum[j - 1] += fnum[j] / N;
				fnum[j] = fnum[j] % N;
			}
			if (fnum[0] >= N)
				break;
		}
		//��� result���� �ּ��� ��Ȳ�� Ž��
		int mini = result[0].first;
		for (int i = 1; i < result.size(); i++) {
			if (result[i].first < mini) {
				mini = result[i].first;
			}
		}
		//�ּ��� ��Ȳ�� �������� �� ���� ����
		for (int i = 0; i < result.size(); i++) {
			if (mini == result[i].first) {
				result_.push_back(result[i]);
			}
		}
	}

	//order�� fnum��° ȭ���� ���� �� tn�� ���ð��� ����
	void getAbleTime(vector<menu> order, vector<int> fnum, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		vector<fuel> sboard;
		for (int i = 0; i < N; i++)
			sboard.push_back(arr[i]);
		vector<vector<fuel>> result;
		//��͸� ���� ���� ����
		int ans = order.size();
		//result���� ȭ���� ��� ����� ���� ����
		subAble(sboard, order, fnum, tn, 0, ans, result);
		//Ž�� �ʱ� ���� �ִ� ��� ����� ������ ���� ª�����ð��� ��츦 Ž��
		int index = 0, mini = execute(result[0], tn);
		for (int i = 1; i < result.size(); i++) {
			int r = execute(result[i], tn);
			if (r < mini) {
				index = i; mini = r;
			}
		}
		//����
		result_.push_back({ mini,result[index] });
	}

	//DFS�� menu�� �ִ� ��� ����� ���� result�� ����
	void subAble(vector<fuel>simul, vector<menu>&order, vector<int>&num, int& tn, int dep, int& ans, vector<vector<fuel>>& result) {
		//simul : ���� ȭ�� ����, order : �޴�����, num : �� ȭ�� ��ȣ, tn : ���̺��ȣ, dep : ����, ans : ������ ��
		if (dep == ans) {//������ Ž���� ��� ���� ȭ�����¸� ����
			result.push_back(simul);
		}
		else {
			//m : ���� �޴�, fnum : �޴��� �� ȭ�� ��ȣ, index : ���� ȭ����Ȳ���� m�� �� �� �ִ� �ε��� ��ȣ����
			menu m = order[dep]; int fnum = num[dep]; vector<int> index = simul[fnum].getAble(m);
			//��� �ε��� ���տ� ����
			for (int i = 0; i < index.size(); i++) {
				vector<fuel> s = simul;
				if (index[i] == -1) {//index�� -1�̸� �ڿ� �ְ� ���
					s[fnum].insertBack(m, tn);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
				else {//-1�� �ƴϸ� index�� �ְ� ���
					s[fnum].insertSimul(m, tn, index[i]);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	//�ֹ濡 �޴��� �ִ°�?
	bool isIn(menu a) {
		for (int i = 0; i < N; i++) {
			if (arr[i].isIn(a))
				return true;
		}
		return false;
	}

	int execute(vector<fuel> s, int tn_) {
		//first : ������ ó�� ���� ��, second ; ������������ ���� ��
		int first = -1, last = -1;

		for (int i = 0; i < N; i++) {
			int time = 0;//����ð�
			while (s[i].getSize()) {
				pair<menu, vector<int>> t = s[i].popOrder();
				vector<int> v = t.second;
				time += t.first.getTime();
				for (int k = 0; k < v.size(); k++) {
					int tn = v[k];//���̺� ��ȣ
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
		//first : ������ ó�� ���� ��, second ; ������������ ���� ��
		vector<pair<int, int>> r;
		for (int i = 0; i < T; i++)
			r.push_back({ -1,-1 });

		for (int i = 0; i < N; i++) {
			int time = 0;//����ð�
			while (arr[i].getSize()) {
				pair<menu, vector<int>> t = arr[i].popOrder();
				vector<int> v = t.second;
				time += t.first.getTime();
				for (int k = 0; k < v.size(); k++) {
					int tn = v[k];//���̺� ��ȣ
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

	//ȭ������ f�� tn�� ���ؼ� under�� over����
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

	void print() {
		for (int i = 0; i < N; i++) {
			cout << i << "�� ȭ��";
			arr[i].print();
		}
		cout << endl;
	}
};

int main() {
	int N, M, cnt = 0;
	vector<double> timeTable[3];
	clock_t start, end;
	cout << "ȭ���� ���� : ";
	cin >> N;
	cout << "�޴��� ���� : ";
	cin >> M;

	menu *board = new menu[M];
	kitchen kitchen0(N), kitchen1(N), kitchen2(N);
	for (int i = 0; i < M; i++) {
		int j;
		string name;
		cout  << i << "��° �޴��� �̸�, �ִ����� ������ �ð� : ";
		cin >> board[i].name;
		cin >> j;
		board[i].setMax(j);
		cin >> j;
		board[i].setTime(j);
		board[i].setNum(i);
	}
	cin.ignore();
	//��ºκ�
	for (;;) {
		system("cls");
		cout << "Fast" << endl;
		kitchen0.print();
		cout << "BruteForce�˰���" << endl;
		kitchen1.print();
		cout << "Greedy�˰���" << endl;
		kitchen2.print();
		cout << "insert : ";
		string in;
		vector<menu> order;
		getline(cin, in);
		if (in.size() == 0) {
			cout << "���α׷��� �����մϴ�.\n" << endl;
			double a0 = 0.0, a1 = 0.0, a2 = 0.0;
			double tt[3] = { 0.0 };

			vector<pair<int, int>> r0 = kitchen0.execute(cnt);
			vector<pair<int, int>> r1 = kitchen1.execute(cnt);
			vector<pair<int, int>> r2 = kitchen2.execute(cnt);

			for (int l = 0; l < r0.size(); l++) {
				a0 += r0[l].second - r0[l].first;
				a1 += r1[l].second - r1[l].first;
				a2 += r2[l].second - r2[l].first;
			}

			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < timeTable[k].size(); l++) {
					tt[k] += timeTable[k].at(l);
				}
			}
			cout << "Fast�� ��� ���ð� | ���� �ҿ�ð�\n" << a0 / cnt << " | " << (tt[0] / CLOCKS_PER_SEC) / cnt << "s" << endl;
			cout << "BruteForce�˰����� ��� ���ð� | ���� �ҿ�ð�\n" << a1 / cnt << " | " << (tt[1] / CLOCKS_PER_SEC) / cnt << "s" << endl;
			cout << "Greedy�˰����� ��� ���ð� | ���� �ҿ�ð�\n" << a2 / cnt << " | " << (tt[2] / CLOCKS_PER_SEC) / cnt << "s" << endl;
			return 0;
		}
		for (int i = 0; i < in.size(); i++) {
			int number = 0;
			for (; in[i] != ' ' && i < in.size(); i++) {
				number = number * 10 + (in[i] - '0');
			}
			order.push_back(board[number]);
		}
		start = clock();
		kitchen0.insertFast(order, cnt);
		end = clock();
		timeTable[0].push_back((double)(end - start));
		start = clock();
		kitchen1.insertBrute(order, cnt);
		end = clock();
		timeTable[1].push_back((double)(end - start));
		start = clock();
		kitchen2.insertGreedy(order, cnt);
		end = clock();
		timeTable[2].push_back((double)(end - start));
		start = clock();
		cnt++;
	}
	return 0;
}
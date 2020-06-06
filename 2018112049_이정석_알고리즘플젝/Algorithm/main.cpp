#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <ctime>

using namespace std;

class menu;
class fuel;
class kitchen;

//메뉴 클래스
class menu {
private:
	//최대 조리 개수와 걸리는 시간
	int max = -1;
	int time = -1;

public:
	//음식 식별 번호
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

//화구 클래스
class fuel {
private:
	//화구에 있는 음식들 first : 메뉴 second : 테이블번호vector
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

	//under over에 대해서 menu를 넣을 수 있는 최적의 타이밍을 반환
	int getAroundTiming(menu m, int under, int over, int& index) {
		int t = time, lasttime = time + m.getTime(), i;
		//마지막으로 들어갈 수 있는 index탐색
		for (i = order.size() - 1; i >= 0; i--) {
			if (m == order[i].first&&order[i].second.size() < order[i].first.getMax()) {
				break;
			}
			t -= order[i].first.getTime();
		}
		//t=0일때 = 화구에 메뉴가 존재하지 않는다.
		if (t == 0) {
			index = order.size();
			return lasttime;
		}//있긴 하지만 under이전이다. over가 없다는 의미
		else if (t < under) {
			index = i;
			return t;
		}//over이후일때
		else if (t > over) {
			int t2 = 0;
			int j;
			//under이전이 존재하는지 모름
			for (j = 0; j < order.size(); j++) {
				t += order[j].first.getTime();
				if (m == order[j].first&&order[j].second.size() < order[j].first.getMax()) {
					break;
				}
			}
			//under이전이 존재하지 않을때
			if (t == t2) {
				index = i;
				return t;
			}
			else {//under이전이 존재할 때 변화하는 정도가 적을 때로 설정
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

	//맨뒤에 추가한다
	void insertBack(menu m, int tn) {
		pair<menu, vector<int>> p;
		p.first = m; p.second.push_back(tn);
		order.push_back(p);
		time += m.getTime();
	}

	//메뉴 m을 가장 빠른자리에 넣는다.
	void insertFast(menu m, int tn) {
		int i;
		//앞에서부터 들어갈 수 있는 자리를 탐색
		for (i = 0; i < order.size(); i++) {
			if (m == order[i].first&&order[i].second.size() != order[i].first.getMax())
				break;
		}
		//없으면 insertBack
		if (i == order.size())
			insertBack(m, tn);
		else {//있으면 테이블 번호 추가
			order[i].second.push_back(tn);
		}
	}

	//메뉴 m을 index에 넣는다.
	void insertAround(menu m, int tn, int index) {
		//index가 맨뒤 부분일때 insertBack
		if (index >= order.size()) {
			insertBack(m, tn);
		}
		else {//맨뒤 부분이 아니면 해당 index에 번호추가
			order[index].second.push_back(tn);
		}
	}

	//메뉴 m을 index에 넣는다.(이미 예외처리 된 이후)
	void insertSimul(menu m, int tn, int index) {
		order[index].second.push_back(tn);
	}

	//화구에 menu가 있는가?
	bool isIn(menu m) {
		for (int i = 0; i < order.size(); i++) {
			if (m == order[i].first)
				return true;
		}
		return false;
	}

	//order크기 반환
	int getSize() {
		return order.size();
	}

	//맨 앞에 있는 음식 상태를 반환
	pair<menu, vector<int>> popOrder() {
		pair<menu, vector<int>> r = order[0];
		order.erase(order.begin());
		return r;
	}

	//메뉴 m이 들어갈 수 있는 index번호들 반환 맨뒤일 경우 -1
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

	//unter <= t <= over를 만족하는 메뉴m이 있는가?
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

	//tableNumber의 하한선 반환
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

	//tableNumber의 상한선 반환
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
		for (int i = 0; i < order.size(); i++) {
			cout << "{" << order[i].first.number << ", ";
			for (int j = 0; j < order[i].second.size(); j++) {
				cout << order[i].second.at(j) << ' ';
			}
			cout << "}\n";
		}
	}
};

class kitchen {
private:
	//arr은 화구 배열, N은 화구의 개수
	fuel *arr;
	int N;

public:
	kitchen(int N) { arr = new fuel[N]; this->N = N; }
	kitchen(kitchen& k) {//kitchen의 깊은복사 생성자
		N = k.N;
		arr = new fuel[N];
		for (int i = 0; i < N; i++) {
			arr[i] = k.arr[i];
		}
	}

	//메뉴 m이 들어갈 수 있는 자리 중 가장 빠른 화구번호를 반환
	int getFastFuel(menu m) {
		//0부터 N-1까지의 화구에서 빠른 타이밍을 갱신해나간다.
		int r = 0, t = arr[0].getFastTiming(m);
		for (int i = 1; i < N; i++) {
			int ct = arr[i].getFastTiming(m);
			if (t > ct) {
				r = i; t = ct;
			}
		}
		return r;
	}

	//tn에서 주문한 메뉴들을 가장 빠른 위치에 하나씩 넣는다.
	void insertFast(vector<menu> order, int tn) {
		sort(order.begin(), order.end());
		for (int i = 0; i < order.size(); i++) {
			arr[getFastFuel(order[i])].insertFast(order[i], tn);
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	void insertGreedy(vector<menu> order, int tn) {
		//1. 현재 화구에 없는 메뉴 2. 오래걸리는 메뉴 순으로
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
		//화구에 넣고 완료시간을 기준으로
		int findex = getFastFuel(firstIn), ftime = arr[findex].getFastTiming(firstIn) + firstIn.getTime();
		arr[findex].insertFast(firstIn, tn);
		//초기 under, over로 설정한다.
		int under = ftime, over = ftime;
		for (int i = 0; i < order.size(); i++) {
			//하나씩 화구에 넣은 후 under와 over를 갱신한다.
			int index = -1;
			arr[getAroundFuel(order[i], under, over, index)].insertAround(order[i], tn, index);
			under = getUnder(tn); over = getOver(tn);
		}
	}

	//기준을 만족하는 화구번호를 반환
	int getAroundFuel(menu m, int under, int over, int& index) {
		/* 기준
		1. under <= t <= over를 만족하는 자리가 있는 화구번호
		2. 만약 다 만족하지 않으면 over-under가 가장 작은 화구번호
		*/
		vector<pair<int, int>> r;//first : 화구번호, second : index
		for (int i = 0; i < N; i++) {
			//under <= t <= over를 만족하는 화구가 있으면
			int in = -1;
			if (arr[i].isIn(m, under, over, in)) {
				//return에 추가
				r.push_back({ i,in });
			}
		}
		//결과가 존재한다
		if (r.size() > 0) {
			index = r[0].second;
			return r[0].first;
		}
		//결과가 없다 over or under를 갱신해야 하는 상황
		vector<pair<pair<int, int>, pair<int, int>>> sr;
		for (int i = 0; i < N; i++) {
			int in = -1;
			//z : 만약 i 화구에 넣게 된다면 최적의 시간을 반환
			int z = arr[i].getAroundTiming(m, under, over, in);
			//under와 over의 대소관계는 명확하므로 m2가 새로운 over m1이 새로운 under
			int m1 = min(z, under), m2 = max(z, over);
			//r을 재활용 first.first : over-under 시간간격 , first.second : index, vectorIndex = 화구index
			//r을 재활용 second.first : under , second.second : over
			sr.push_back({ { abs(m1 - m2),in },{m1,m2} });
		}

		//시간 간격이 제일 짧은 경우를 탐색
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

	//전체 화구들에 대한 under반환
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

	//전체 화구들에 대한 over반환
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
		//result first : 시간 간격, second : 그 때의 화구상태
		vector<pair<int, vector<fuel>>> result;
		//result에는 order에 대한 fuel의 모든 경우의 수와 그에 대한 over-under가 들어있다.
		getBruteTime(order, tn, result);
		//여러개의 시간 간격과 그 때의 상황이 있다.
		//필터링
		int mini = result[0].first;
		int under, over, miniIndex = 0;
		//처음 상황에서 under와 over를 구한다.
		getMidTime(under, over, result[0].second, tn);
		for (int i = 1; i < result.size(); i++) {
			//시간 간격이 더 짧은 경우 갱신
			if (result[i].first < mini) {
				mini = result[i].first;
				getMidTime(under, over, result[i].second, tn);
				miniIndex = i;
			}else if (result[i].first == mini) {
				//시간 간격이 같으나 over-under가 다를 수 있음
				int a, b;
				getMidTime(a, b, result[i].second, tn);
				//더 짧은 경우를 찾을 경우 갱신
				if (abs(over - under) > abs(b - a)) {
					under = a; over = b;
					miniIndex = i;
				}
				else if (abs(over - under) == abs(b - a)) {
					//over-under마저 같으면 over가 짧을 때로 갱신
					if (over > b) {
						under = a; over = b;
						miniIndex = i;
					}
				}
			}
		}

		//최종적으로 화구상태 갱신
		for (int i = 0; i < N; i++) {
			arr[i] = result[miniIndex].second.at(i);
		}
	}

	//result_에 모든 경우의 수를 저장
	void getBruteTime(vector<menu> order, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		//order 넣을 메뉴 순서
		vector<int> fnum(order.size(), 0);
		vector<pair<int, vector<fuel>>> result;
		//무한 반복
		for (int i = 0;; i++) {
			//현재 fnum상태에서 모든 경우의 수를 추가
			getAbleTime(order, fnum, tn, result);
			//다음 fnum상태로 전환
			fnum[fnum.size() - 1]++;
			for (int j = fnum.size() - 1; j > 0; j--) {
				fnum[j - 1] += fnum[j] / N;
				fnum[j] = fnum[j] % N;
			}
			if (fnum[0] >= N)
				break;
		}
		//모든 result에서 최소인 상황을 탐색
		int mini = result[0].first;
		for (int i = 1; i < result.size(); i++) {
			if (result[i].first < mini) {
				mini = result[i].first;
			}
		}
		//최소인 상황이 여러가지 일 수도 있음
		for (int i = 0; i < result.size(); i++) {
			if (mini == result[i].first) {
				result_.push_back(result[i]);
			}
		}
	}

	//order를 fnum번째 화구에 넣을 때 tn의 대기시간을 저장
	void getAbleTime(vector<menu> order, vector<int> fnum, int& tn, vector<pair<int, vector<fuel>>>& result_) {
		vector<fuel> sboard;
		for (int i = 0; i < N; i++)
			sboard.push_back(arr[i]);
		vector<vector<fuel>> result;
		//재귀를 끝낼 깊이 설정
		int ans = order.size();
		//result에는 화구의 모든 경우의 수가 저장
		subAble(sboard, order, fnum, tn, 0, ans, result);
		//탐색 초기 설정 넣는 모든 경우의 수에서 가장 짧은대기시간의 경우를 탐색
		int index = 0, mini = execute(result[0], tn);
		for (int i = 1; i < result.size(); i++) {
			int r = execute(result[i], tn);
			if (r < mini) {
				index = i; mini = r;
			}
		}
		//저장
		result_.push_back({ mini,result[index] });
	}

	//DFS로 menu를 넣는 모든 경우의 수를 result에 저장
	void subAble(vector<fuel>simul, vector<menu>&order, vector<int>&num, int& tn, int dep, int& ans, vector<vector<fuel>>& result) {
		//simul : 현재 화구 상태, order : 메뉴순서, num : 들어갈 화구 번호, tn : 테이블번호, dep : 깊이, ans : 깊이의 끝
		if (dep == ans) {//끝까지 탐색한 경우 현재 화구상태를 저장
			result.push_back(simul);
		}
		else {
			//m : 현재 메뉴, fnum : 메뉴가 들어갈 화구 번호, index : 현재 화구상황에서 m이 들어갈 수 있는 인덱스 번호집합
			menu m = order[dep]; int fnum = num[dep]; vector<int> index = simul[fnum].getAble(m);
			//모든 인덱스 집합에 대해
			for (int i = 0; i < index.size(); i++) {
				vector<fuel> s = simul;
				if (index[i] == -1) {//index가 -1이면 뒤에 넣고 재귀
					s[fnum].insertBack(m, tn);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
				else {//-1이 아니면 index에 넣고 재귀
					s[fnum].insertSimul(m, tn, index[i]);
					subAble(s, order, num, tn, dep + 1, ans, result);
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------------
	//주방에 메뉴가 있는가?
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

	//화구상태 f와 tn에 대해서 under와 over갱신
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
			cout << i << endl;
			arr[i].print();
			cout << endl;
		}
	}
};

int main() {
	int N, M, T;
	double **timeTable;
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

	cout << "insert 회수 : ";
	cin >> T;
	timeTable = new double*[3];
	for (int i = 0; i < 3; i++) {
		timeTable[i] = new double[T];
	}
	for (int i = 0; i < T; i++) {
		int j;
		cout << i << "번째 insert의 메뉴개수 : ";
		cin >> j;
		vector<menu> order;
		while (j--) {
			int number;
			cin >> number;
			order.push_back(board[number]);
		}
		start = clock();
		kitchen0.insertFast(order, i);
		//kitchen0.print();
		end = clock();
		timeTable[0][i] = (double)(end - start);
		start = clock();
		kitchen1.insertBrute(order, i);
		kitchen1.print();
		end = clock();
		timeTable[1][i] = (double)(end - start);
		start = clock();
		kitchen2.insertGreedy(order, i);
		kitchen2.print();
		end = clock();
		timeTable[2][i] = (double)(end - start);
		start = clock();
	}
	cout << "-------------------------------\n";
	vector<pair<int, int>> r0 = kitchen0.execute(T);
	//kitchen0.print();
	double average0 = 0.0;
	cout << "[Fast]\n";
	for (int i = 0; i < r0.size(); i++) {
		cout << i << "번째 insert 대기시간 : " << r0[i].second - r0[i].first << endl;
		average0 += r0[i].second - r0[i].first;
	}
	cout << "평균 대기시간 : " << average0 / T << endl;
	cout << "-------------------------------\n";
	vector<pair<int, int>> r1 = kitchen1.execute(T);
	//kitchen1.print();
	double average1 = 0.0;
	cout << "[BruteForce]\n";
	for (int i = 0; i < r1.size(); i++) {
		cout << i << "번째 insert 대기시간 : " << r1[i].second - r1[i].first << endl;
		average1 += r1[i].second - r1[i].first;
	}
	cout << "평균 대기시간 : " << average1 / T << endl;
	cout << "-------------------------------\n";
	vector<pair<int, int>> r2 = kitchen2.execute(T);
	//kitchen2.print();
	double average2 = 0.0;
	cout << "[Greedy]\n";
	for (int i = 0; i < r2.size(); i++) {
		cout << i << "번째 insert 대기시간 : " << r2[i].second - r2[i].first << endl;
		average2 += r2[i].second - r2[i].first;
	}
	cout << "평균 대기시간 : " << average2 / T << endl;
	cout << "-------------------------------\n[평균실행시간]\n";
	double Taverage0 = 0.0, Taverage1 = 0.0, Taverage2 = 0.0;
	for (int j = 0; j < T; j++) {
		Taverage0 += timeTable[0][j];
		Taverage1 += timeTable[1][j];
		Taverage2 += timeTable[2][j];
	}
	Taverage0 /= T, Taverage1 /= T, Taverage2 /= T;
	cout << "Fast : " << Taverage0 << "ms" << endl;
	cout << "BruteForce : " << Taverage1 << "ms" << endl;
	cout << "Greedy : " << Taverage2 << "ms" << endl;
	cout << "-------------------------------\n";
	cin >> T;
	/*
	kitchen kitchen3(kitchen1), kitchen4(kitchen1), kitchen5(kitchen1);
	int time3, time4, time5;
	{
		int j;
		cout << "마지막 insert의 메뉴개수 : ";
		cin >> j;
		vector<menu> order;
		while (j--) {
			int number;
			cin >> number;
			order.push_back(board[number]);
		}
		start = clock();
		kitchen3.insertFast(order, T);
		end = clock();
		time3 = end - start;
		start = clock();
		kitchen4.insertBrute(order, T);
		end = clock();
		time4 = end - start;
		start = clock();
		kitchen5.insertGreedy(order, T);
		end = clock();
		time5 = end - start;
	}
	vector<pair<int, int>> r3 = kitchen3.execute(T + 1);
	vector<pair<int, int>> r4 = kitchen4.execute(T + 1);
	vector<pair<int, int>> r5 = kitchen5.execute(T + 1);
	cout << "Fast 소요시간 및 대기시간 : " << time3 << "ms, " << r3[T].second - r3[T].first << endl;
	cout << "BruteForce알고리즘 소요시간 및 대기시간 : " << time4 << "ms, " << r4[T].second - r4[T].first << endl;
	cout << "Greedy알고리즘 소요시간 및 대기시간 : " << time5 << "ms, " << r5[T].second - r5[T].first << endl;
	*/
	return 0;
}
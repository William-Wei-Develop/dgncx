#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <ctime> 
#pragma warning(disable:4996)
using namespace std;
void SetPos(COORD a) { HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleCursorPosition(out, a); }
void SetPos(int i, int j) { COORD pos = { (SHORT)i, (SHORT)j }; SetPos(pos); }
void color(int a) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a); }
template <typename T, typename Container> class pair_t {
public:
	T first;
	Container second;
	pair_t(T x, Container y) { first = x, second = y; }
	pair_t() { first = second = 0; }
	pair_t<T, Container> mkpair(T fi, Container se) { pair_t<T, Container> p; p.first = fi; p.second = se; return p; }
	pair_t<T, Container> operator-(pair_t<T, Container> rhs) { return mkpair(first - rhs.first, second - rhs.second); }
	pair_t<T, Container> operator+(pair_t<T, Container> rhs) { return mkpair(first + rhs.first, second + rhs.second); }
	pair_t<T, Container> operator/(pair_t<T, Container> rhs) { return mkpair(first / rhs.first, second / rhs.second); }
	pair_t<T, Container> operator*(pair_t<T, Container> rhs) { return mkpair(first * rhs.first, second * rhs.second); }
	bool operator==(pair_t<T, Container> rhs) { return first == rhs.first && second == rhs.second; }
	bool operator<(pair_t<T, Container> rhs) const {
		if (first != rhs.first) return first < rhs.first;
		return second < rhs.second;
	}
	bool operator>(pair_t<T, Container> rhs) const {
		if (first != rhs.first) return first > rhs.first;
		return second > rhs.second;
	}
	pair_t<T, Container> operator&(pair_t<T, Container> rhs) const { return mkpair(first & rhs.first, second & rhs.second); }
	pair_t<T, Container> operator|(pair_t<T, Container> rhs) const { return mkpair(first | rhs.first, second | rhs.second); }
	pair_t<T, Container> operator%(pair_t<T, Container> rhs) const { return mkpair(first % rhs.first, second % rhs.second); }
	pair_t<T, Container> operator^(pair_t<T, Container> rhs) const { return mkpair(first ^ rhs.first, second ^ rhs.second); }
	auto operator[](int typ) {
		if (typ > 1 || typ < 0) return -0xff;
		if (typ == 0) return first;
		return second;
	}
};
class Snake {
private:
	char snakemap[30][30] = {
		"***********************",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"*                     *",
		"***********************"
	};
	vector<pair_t<int, int> > snakebody;
	inline int SZ() { return (int)snakebody.size(); }
	inline bool POSIN(int x, int y) { return (x >= 1 && x < 23 && y >= 1 && y < 22); }
	const int dx[4] = { 0,-1,0,1 }, dy[4] = { -1,0,1,0 };
	int curdfs = 0;
	int foodx = 0, foody = 0;
	inline void add() {
		if (SZ() >= 2) {
			pair_t<int, int> pmain = snakebody[SZ() - 1] - snakebody[SZ() - 2];
			snakebody.push_back(snakebody[SZ() - 1] + pmain);
		}
		else snakebody.push_back(pair_t<int, int>(snakebody[SZ() - 1].first + dx[curdfs], snakebody[SZ() - 1].second + dy[curdfs]));
	}
public:
	inline void change_sb() { SetPos(0, 28); }
	bool _exit__ = false;
	inline void move() {
		// the NOW curdfs means that the SNAKE's HEADER MOVING.
		// iterator: v[i] = v[i-1]. v[0] = {v[0].first + dx[curdfs], v[0].second + dy[curdfs]}
		SetPos(snakebody[SZ() - 1].second, snakebody[SZ() - 1].first + 5);
		printf(" ");
		for (int i = SZ() - 1; i >= 1; i--) snakebody[i] = snakebody[i - 1];
		snakebody[0] = pair_t<int, int>(snakebody[0].first + dx[curdfs], snakebody[0].second + dy[curdfs]);
	}
	void init() {
		printf("WASD??????????????????, ????????????, #?????????, *????????????, @????????????\nv1.2.3 ??????????????????:\n1.???????????????, O(24)\n2.?????????????????????\n???[Q]??????\n");
		curdfs = rand() % 4;
		int wx = rand() % 18, wy = rand() % 18;
		while (wx < 6) wx = rand() % 18;
		while (wy < 6) wy = rand() % 18;
		snakebody.clear();
		snakebody.push_back(pair_t<int, int>(wx, wy));
		food();
		SetPos(0, 5);
		for (int i = 0; i < 24; i++) printf("%s\n", &snakemap[i]);
		SetPos(foody, foodx + 5);
		printf("@");
	}
	inline void draw() {
		if (!POSIN(snakebody[0].first, snakebody[0].second) || (snakebody[0] == snakebody[SZ() - 1] && SZ() > 1)) {
			system("cls"); printf("You Lose..."); system("pause"); system("cls");
			init();
			return;
		}
		SetPos(snakebody[0].second, snakebody[0].first + 5), printf("#");
	}
	inline void pd() { if (snakebody[0].first == foodx && snakebody[0].second == foody) add(), food(), SetPos(foody, foodx + 5), printf("@"); }
	inline void food() { foodx = rand() % 21, foody = rand() % 22; if (foodx == 0 || foody == 0) food(); }
	inline void kz() {
		char ch = _getch();
		if (ch == 'q' || ch == 'Q') { _exit__ = true; return; }
		else if (ch == 'w' || ch == 'W') curdfs = 1;
		else if (ch == 'a' || ch == 'A') curdfs = 0;
		else if (ch == 's' || ch == 'S') curdfs = 3;
		else if (ch == 'd' || ch == 'D') curdfs = 2;
		move(); pd(); draw(); change_sb();
	}
};
void playsnake() {
	int _time = 0xff >> 1, tmp;
	system("cls");
	srand((unsigned int)time(0));
	Snake sn;
	system("TITLE Snake");
	cout << "?????????????????????: (0?????????): ";
	cin >> tmp;
	if (tmp) _time = (tmp > 0 && tmp < 0xffff) ? tmp : _time;
	system("cls");
	sn.init();
	sn.draw();
	while (true)
		if (_kbhit()) sn.kz();
		else if (sn._exit__) return;
		else {
			for (int i = 1; i <= 500; i++)
				if (_kbhit()) sn.kz();
				else if (sn._exit__) return;
				else {
					Sleep(_time);
					sn.move(), sn.pd(), sn.draw(); sn.change_sb();
				}
		}
}
void snake() {
	system("cls");
	printf("\n\n\n\n\n");
loop_snake_head:
	system("cls");
	color(10);
	SetPos(1, 1);
	printf("|???????????????| ???   ??????"); color(11);
	SetPos(16, 1); printf("[`]"); color(14);
	printf("\n\n ????????????????????????????????????"); SetPos(1, 5);
	printf("|????????????| [a]"); SetPos(1, 7);
	printf("|????????????| [s]"); SetPos(1, 9);
	printf("|????????????| [d]"); SetPos(1, 11);
	printf("|????????????| [f]");
	char c;
	while (1)
		if (_kbhit()) {
			c = _getch();
			switch (c) {
			case 'a': {
				system("cls"); printf("?????????...");
				Sleep(2000);
				goto loop_snake_head;
			}
			case 's': {
				playsnake();
				goto loop_snake_head;
			}
			case 'd': {
				system("cls"); printf("?????????...");
				Sleep(2000);
				goto loop_snake_head;
			}
			case 'f': {
				system("cls");
				printf("?????????...");
				Sleep(2000);
				goto loop_snake_head;
			}
			case '`':
				goto loop_snake_tail;
			}
		}
loop_snake_tail:
	return;
}
int main() { snake(); }
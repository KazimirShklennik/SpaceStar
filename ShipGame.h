
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <conio.h>

using namespace std;

class shipGame {
private: int x, y, ship, asteroid_x, asteroid_y, dx, dy, lastpos_x, lastpos_y, points, level;
		 bool go, deatroy;
		 char**map;
		 char ch;

		  std::vector<int> x_ship;
		 std::vector<int> y_ship;

public: shipGame();
		~shipGame();
		void start_game();
		void game_over();
		void move();
		void input();
		void asteroid();
		void game();
		void refresh();
		void clear();
		void draw();
		void hidecursor();
		void test();
		int check_over();
protected:
	void SetDefaultConsoleColor(HANDLE hConsole)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
};


void shipGame::hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}




//нициализация данных

shipGame::shipGame() {
	go = true;
	deatroy = false;
	level = 100;
	dx = 0;
	dy = 1;
	x = 6;
	y = 25;
	asteroid_x = 1;
	asteroid_y = 1;
	ship = 1;
	points = 1;
	ch = '>';

}

shipGame::~shipGame() {
	for (int i = 0; i < 26; i++) {
		delete[] map[i];
	}
	delete[] map;
}

//старт игры

void shipGame::start_game() {
	hidecursor();
	map = new char*[26];
	for (int i = 0; i < 26; i++) {
		map[i] = new char[81];
	}
	draw();
}

//перерисовка консоли

void shipGame::draw() {

	//зарисовка верхней и нижней стены

	for (int j = 1; j <= 80; j++) {
		map[1][j] = char(178);
		map[25][j] = char(178);
	}

	//зарисовка правой и леыой стены

	for (int i = 1; i <= 25; i++) {
		map[i][1] = char(178);
		map[i][80] = char(178);
	}
	for (int i = 2; i <= 24; i++) {
		for (int j = 2; j <= 79; j++) {
			map[i][j] = ' ';
		}
	}
	//зарисовка хвоста и головы змеи

	map[x][y] = char(254);
	for (int i = 22; i <= 24; i++) {
		y_ship.push_back(i);
	}
	for (int i = 0; i < ship; i++) {
		x_ship.push_back(x);
	}
	for (int i = 0; i < ship; i++) {
		map[x_ship[i]][y_ship[i]] = '#';
	}

	//показать всю карты

	for (int i = 1; i <= 25; i++) {
		for (int j = 1; j <= 80; j++) {
			if (j == 40)
				cout << map[i][j] << endl;
			else
				cout << map[i][j];
		}
	}

	//generate apple
	asteroid();

	//перемещение
	move();
}

void shipGame::refresh() {

	//обновить карту с новой позицией

	map[x][y] = char(254);
	for (int i = 0; i < ship; i++) {
		map[x_ship[i]][y_ship[i]] = ch;
	}

	//показать обновленную карту

	for (int i = 1; i <= 25; i++) {
		for (int j = 1; j <= 80; j++) {
			if (j == 80)
				cout << map[i][j] << endl;
			else
				cout << map[i][j];
		}
	}

	//продолжать обнаруживать движение после обновления
	move();
}

//apple random generation
void shipGame::asteroid() {
	srand((unsigned)time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int z = 50;
	for (int i = 0; i < z; i++) {	
		asteroid_x = 1 + rand() % (24 - 2 + 1);
		asteroid_y = 1 + rand() % (79 - 2 + 1);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		map[asteroid_x][asteroid_y] = '*';
		//SetDefaultConsoleColor(hConsole);
	}
	z += 50;
}

//вылет за границу поля

int shipGame::check_over() {
	if ((x == 1) || (x == 25) || (y == 1) || (y == 80)) {
		return 1;
	}
	for (int i = 0; i < ship - 1; i++) {
		if ((x_ship[i] == x) && (y_ship[i] == y))
			return 1;
	}
	return 0;
}

//игра закончена.

void shipGame::game_over() {
	system("cls");
	Sleep(1000);
	cout << "Game Over!!!" << endl;
	Sleep(50);
	cout << "You earned " << points << " points!!!" << endl;
}

void shipGame::clear() {
	system("cls");
}

//перемещение 

void shipGame::move() {

	//цикл пока игра не закончится
	while (go == true) {

		//обновить положение корабля, если нажата клавиша
		if (_kbhit()) {
			input();
		}
		//обновить позицию

		cout << endl;
		lastpos_x = x;
		lastpos_y = y;
		x = x + dx;
		y = y + dy;

		//сгенерировать астероид
		if ((x == asteroid_x) && (y == asteroid_y)) {
			points = points + 10;
			deatroy = true;
			map[asteroid_x][asteroid_y] = '*';
			asteroid();
		}

		if (deatroy == true) {
			game_over();
			go = false;
		}

		//создать корабль

		map[x_ship[0]][y_ship[0]] = ' ';
		for (int i = 0; i < ship - 1; i++) {
			x_ship[i] = x_ship[i + 1];
			y_ship[i] = y_ship[i + 1];
		}
		x_ship[ship - 1] = lastpos_x;
		y_ship[ship - 1] = lastpos_y;
		if (check_over()) {
			game_over();
			go = false;
			break;
		}

		//увеличение скорости корабля

		if (points > 50 && points <= 100) {
			level = 75;
		}
		else if (points > 100 && points <= 150) {
			level = 50;
		}
		else if (points > 150) {
			level = 25;
		}
		Sleep(level);

		//обновить карту

		clear();
		refresh();
	}
}

//управление

void shipGame::input() {
	if (_kbhit()) {
		char in = _getch();
		switch (in) {
		case 'd':
		{
			dx = 0;
			dy = 1;
			ch = '>';
			break;
		}
		case 'w':
		{
			dx = -1;
			dy = 0;
			ch = '^';
			break;
		}
		case 'a':
		{
			dx = 0;
			dy = -1;
			ch = '<';
			break;
		}
		case 's':
		{
			dx = 1;
			dy = 0;
			ch = '|';
			break;
		}
		}
	}
}

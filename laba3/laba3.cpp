// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>


using namespace std;

void gotoxy(int x, int y) {
	COORD p{ x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

const int BORDER = 100;
const int EMPTY = 0;
const int MINE = 10;

class Field {
private:
	int SIZE;
	vector <vector<int>> field;
public:
	Field() {
		SIZE = 10;
	}
	void initField() {
		for (int i = 0; i < SIZE; i++) {
			vector<int> temp;
			for (int j = 0; j < SIZE; j++) {
				if (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1)
					temp.push_back(BORDER);
				else
					temp.push_back(EMPTY);
			}
			field.push_back(temp);
		}
	}

	void Show() {
		for (int i = 0; i < SIZE; i++) {
			vector<int> temp;
			for (int j = 0; j < SIZE; j++) {

				if (field[i][j] == BORDER)
					cout << "#";
				else if (field[i][j] == EMPTY)
					cout << " ";
				else if (field[i][j] == MINE)
					cout << "*";
			}
			cout << endl;
		}
	}

	void placeMines(int mines) {
		if (mines >= (SIZE - 2) * (SIZE - 2)) return;
		for (int i = 0; i < mines; i++) {
			int x = 0;
			int y = 0;
			do {
				x = rand() % (SIZE - 2) + 1;
				y = rand() % (SIZE - 2) + 1;
			} while (field[x][y] == MINE);
			field[x][y] = MINE;
		}
	}
};
class Game {
private:
	void Logo() {
		gotoxy(40,10);
		cout << "Сапер" << endl;
		Sleep(2000);
		system("cls");
	}
public:
	void run() {
		//Logo();
		cout << "Начать игру" << endl;
		Field field;
		field.initField();
		field.placeMines(5);
		field.Show();
	}
};

int main()
{
	srand(time(0));
	Game game;
	game.run();
}

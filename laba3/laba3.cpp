// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <vector>


using namespace std;

void gotoxy(int x, int y) {
	COORD p{ x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

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
					temp.push_back(100);
				else
					temp.push_back(0);
			}
			field.push_back(temp);
		}
	}

	void Show() {
		for (int i = 0; i < SIZE; i++) {
			vector<int> temp;
			for (int j = 0; j < SIZE; j++) {

				if (field[i][j] == 100)
					cout << "#";
				else if (field[i][j] == 0)
					cout << " ";
			}
			cout << endl;
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
		field.Show();
	}
};

int main()
{

	Game game;
	game.run();
}

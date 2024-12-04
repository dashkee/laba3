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

class Field {
private:
	const int BORDER = 100;// граница поля
	const int EMPTY = 0;//пустая ячейка
	const int MINE = 10;//мина
	int SIZE;//размер поля включая границы
	vector <vector<int>> field;
public:
	Field() {
		SIZE = 5;
	}
	//функция инициализации поля
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
	//функция вывода поля
	void Show() {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (field[j][i] == BORDER)
					cout << "#";
				else if (field[j][i] == EMPTY)
					cout << " ";
				else if (field[j][i] == MINE)
					cout << "*";
				else if (field[j][i] >= 1 && field[j][i] <= 8)
					cout << field[j][i];
			}
			cout << endl;
		}
	}
	//функция расстановки мин
	void placeMines(int mines) {
		//проверка на допустимое количество мин
		if (mines >= (SIZE - 2) * (SIZE - 2)) return;
		for (int i = 0; i < mines; i++) {
			int x = 0;
			int y = 0;
			//поиск пустой ячейки, не занятой миной
			do {
				x = rand() % (SIZE - 2) + 1;
				y = rand() % (SIZE - 2) + 1;
			} while (field[x][y] == MINE);
			field[x][y] = MINE;
		}
	}
	//функция расстановки чисел
	void setDigits() {
		int cnt = 0;
		for (int i = 1; i < SIZE - 1; i++) {
			for (int j = 1; j < SIZE - 1; j++) {
				if (field[j][i] == MINE)
					continue;
				if (field[j+1][i] == MINE)
					cnt++;
				if (field[j-1][i] == MINE)
					cnt++;
				if (field[j][i + 1] == MINE)
					cnt++;
				if (field[j][i -1] == MINE)
					cnt++;
				if (field[j+1][i + 1] == MINE)
					cnt++;
				if (field[j-1][i - 1] == MINE)
					cnt++;
				if (field[j-1][i + 1] == MINE)
					cnt++;
				if (field[j+1][i - 1] == MINE)
					cnt++;
				field[j][i] = cnt;
				cnt = 0;
			}
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
		field.placeMines(3);
		field.setDigits();
		field.Show();
	}
};

int main()
{
	srand(time(0));
	Game game;
	game.run();
}

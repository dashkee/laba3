﻿// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <conio.h>
#include <stack>

using namespace std;

void gotoxy(int x, int y) {
	COORD p{ x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

enum ConsoleColor
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARKGRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

//устанавливается цвет текста и подложки консоли
void setColor(int background, int text)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

class Field {
private:
	const int EMPTY = 0;//пустая ячейка
	const int MINE = 10;//мина
	int SIZE;//размер поля включая границы
	const int BORDER = 100;// граница поля
	vector <vector<int>> field;
	vector <vector<int>> mask;
	const int FLAG = 11; // флаг
public:
	Field() {
		SIZE = 10;
	}
	//функция открытия ячейки
	int openCell(int x, int y) {
		int result = 1;

		// Проверяем, что ячейка не помечена флагом
		if (mask[x][y] != FLAG) {
			mask[x][y] = 1; // Открываем ячейку
			// Проверяем содержимое ячейки
			if (field[x][y] == MINE) {
				result = 10; // Ячейка с миной
			}
			else if (field[x][y] == EMPTY) {
				result = 0; // Пустая ячейка
			}
			Show(); // Обновляем отображение
		}

		return result; // Возвращаем результат
	}

	bool isMine(int x, int y) {
		if (field[x][y] == MINE) {
			return 10; // Ячейка с миной
		}
	}

	//функция для определения края поля
	bool isBorder(int x, int y) {
		if (x < 0 || x >= SIZE) return false;
		if (y < 0 || y >= SIZE) return false;

		if (field[x][y] == BORDER)
		{
			return true;
		}
		return false;
	}

	//функция инициализации
	void initVec(vector <vector<int>>& vec) {
		for (int i = 0; i < SIZE; i++) {
			vector<int> temp;
			for (int j = 0; j < SIZE; j++) {
				if (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1)
					temp.push_back(BORDER);
				else
					temp.push_back(EMPTY);
			}
			vec.push_back(temp);
		}
	}

	//функция инициализации поля
	void initField() {
		initVec(field);
	}

	//Маска для ячеек
	void initMask() {
		initVec(mask);
	}

	// установка цвета символа
	void coutColor(char ch, int color) {
		setColor(15, color);
		cout << ch;
		setColor(15, 0);
	}

	//функция вывода поля
	void Show() {
		gotoxy(0, 0);
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {

				if (mask[j][i] == FLAG) {
					coutColor('F', 4);
					continue;
				}
				else if (mask[j][i] == EMPTY) {
					cout << ".";
					continue;
				}
				if (field[j][i] == BORDER) {
					coutColor('#', 9);
				}
				//else 
				else if (field[j][i] == EMPTY) cout << " ";
				else if (field[j][i] == MINE) cout << "*";
				else if (field[j][i] == 1) coutColor('1', 1);
				else if (field[j][i] == 2) coutColor('2', 2);
				else if (field[j][i] == 5) coutColor('5', 6);
				else if (field[j][i] == 3) coutColor('3', 4);
				else if (field[j][i] == 4) coutColor('4', 13);
				else if (field[j][i] == 6) coutColor('6', 8);
				else if (field[j][i] == 7) coutColor('7', 10);
				else if (field[j][i] == 8) coutColor('8', 11);
			}
			cout << endl;
		}
	}

	//функция расстановки мин
	void placeMines(int mines, int _x, int _y) {
		//проверка на допустимое количество мин
		if (mines >= (SIZE - 2) * (SIZE - 2)) return;

		for (int i = 0; i < mines; i++) {
			int x = 0;
			int y = 0;

			x = rand() % (SIZE - 2) + 1;
			y = rand() % (SIZE - 2) + 1;
			if (_x == x && _y == y) continue; // Не ставим мину на первую клетку
			if (field[x][y] != MINE)
			{
				field[x][y] = MINE;
			}
		}
	}

	//функция расстановки чисел
	void setDigits() {
		int cnt = 0;
		for (int i = 1; i < SIZE - 1; i++) {
			for (int j = 1; j < SIZE - 1; j++) {
				if (field[j][i] == MINE)
					continue;
				if (field[j + 1][i] == MINE)
					cnt++;
				if (field[j - 1][i] == MINE)
					cnt++;
				if (field[j][i + 1] == MINE)
					cnt++;
				if (field[j][i - 1] == MINE)
					cnt++;
				if (field[j + 1][i + 1] == MINE)
					cnt++;
				if (field[j - 1][i - 1] == MINE)
					cnt++;
				if (field[j - 1][i + 1] == MINE)
					cnt++;
				if (field[j + 1][i - 1] == MINE)
					cnt++;
				field[j][i] = cnt;
				cnt = 0;
			}
		}
	}

	// функция открытия соседних пустых ячеек
	void fill(int px, int py) {
		stack <int> stk;
		stk.push(px);
		stk.push(py);
		int x = 0, y = 0;
		while (true) {
			y = stk.top();
			stk.pop();
			x = stk.top();
			stk.pop();

			if (field[x][y + 1] == EMPTY && mask[x][y + 1] == 0) {
				stk.push(x);
				stk.push(y + 1);
			}
			mask[x][y + 1] = 1;
			if (field[x][y - 1] == EMPTY && mask[x][y - 1] == 0) {
				stk.push(x);
				stk.push(y - 1);
			}
			mask[x][y - 1] = 1;
			if (field[x + 1][y] == EMPTY && mask[x + 1][y] == 0) {
				stk.push(x + 1);
				stk.push(y);
			}
			mask[x + 1][y] = 1;
			if (field[x - 1][y] == EMPTY && mask[x - 1][y] == 0) {
				stk.push(x - 1);
				stk.push(y);
			}
			mask[x - 1][y] = 1;
			if (field[x + 1][y + 1] == EMPTY && mask[x + 1][y + 1] == 0) {
				stk.push(x + 1);
				stk.push(y + 1);
			}
			mask[x + 1][y + 1] = 1;
			if (field[x - 1][y + 1] == EMPTY && mask[x - 1][y + 1] == 0) {
				stk.push(x - 1);
				stk.push(y + 1);
			}
			mask[x - 1][y + 1] = 1;
			if (field[x - 1][y - 1] == EMPTY && mask[x - 1][y + 1] == 0) {
				stk.push(x - 1);
				stk.push(y + 1);
			}
			mask[x - 1][y + 1] = 1;
			if (field[x + 1][y - 1] == EMPTY && mask[x + 1][y - 1] == 0) {
				stk.push(x + 1);
				stk.push(y - 1);
			}
			mask[x + 1][y - 1] = 1;
			if (stk.empty()) break;
		}
		Show();
	}

	void flag(int x, int y) {
			if (mask[x][y] != FLAG) { // Если флаг не установлен
				mask[x][y] = FLAG; // Устанавливаем флаг
			}
			else if (mask[x][y] == FLAG) {
				mask[x][y] = EMPTY; // Убираем флаг, если он уже установлен
			}
	}

		bool checkWin() {
			for (int x = 0; x < SIZE; x++) {
				for (int y = 0; y < SIZE; y++) {
					if (field[x][y] == MINE && mask[x][y] == EMPTY) {
						return false; // Если есть закрытая ячейка без мины, игрок не выиграл
					}
				}
			}
			return true; // Все ячейки без мин открыты, игрок выиграл
		}

};

//класс для символа с клавиатуры
class Keyboard {
private:
	int ch = 0;
public:
	Keyboard() {
		ch = 0;
	}
	void waitKey() {
		ch = _getch();
	}

	int getKey() {
		return ch;
	}
};

// класс для курсора
class Cursor {
private:
	int x = 1;
	int y = 1;
	int tx = 1;
	int ty = 1;
public:
	void save() {
		tx = x;
		ty = y;
	}

	void undo() {
		x = tx;
		y = ty;
	}

	void incX() {
		x++;
	}

	void incY() {
		y++;
	}

	void decX() {
		x--;
	}
	
	void decY() {
		y--;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void move() {
		gotoxy(x, y);
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
	bool firstOpen = true; // Флаг для отслеживания первой открытой ячейки
public:
	void gameOver() {
		gotoxy(40, 10);
		cout << "Game over";
		Sleep(2000);
		gotoxy(0, 15);
		system("pause");
	}

	void win() {
		gotoxy(40, 10);
		cout << "Вы победили!";
		Sleep(2000);
		gotoxy(0, 15);
		system("pause");
	}

	void run() {
		//Logo();
		//cout << "Начать игру" << endl;
		Field field;
		field.initField();
		field.initMask();
		//field.placeMines(10);
		//field.setDigits();
		field.Show();

		Keyboard kb;
		Cursor cs;

		cs.move();
		bool exit = false;
		while (!exit) {
			kb.waitKey();
			cs.save();

			switch (kb.getKey())
			{
			case 77: cs.incX(); break;// вправо
			case 80: cs.incY(); break;// вниз
			case 75: cs.decX(); break;// влево
			case 72: cs.decY(); break;// вверх
			case 32: field.flag(cs.getX(), cs.getY()); field.Show(); break; //
			//нажатие на enter
			case 13: 
				if (firstOpen == true) {
					field.placeMines(10, cs.getX(), cs.getY());
					field.setDigits();
					firstOpen = false;
				}
					int res = field.openCell(cs.getX(), cs.getY());
					if (res != 1) {
						if (res == 10) {
							gameOver();
							exit = true;
						}
						if (res == 0) {
							field.fill(cs.getX(), cs.getY());
						}
					}
					if (field.checkWin()) {
						win();
						exit = true; // Завершаем игру
					}
				break;
			//default:
              //  break; // Игнорируем остальные нажатия
			}
			if (field.isBorder(cs.getX(), cs.getY()))
			{
				cs.undo();
			}
			cs.move();
		}
	}
};

int main()
{
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	setlocale(LC_ALL, "Rus");
	srand(time(0));
	Game game;
	game.run();
}

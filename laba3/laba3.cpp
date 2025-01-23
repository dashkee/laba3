﻿// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>


#define SIZE 25
#define MINES 99

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

class Menu {
private:
	std::vector<std::string> options;
public:
	int selectedOption;
	Menu(){
		options.push_back("Новая игра");
		options.push_back("Настройки");
		options.push_back("Статистика");
		options.push_back("Справка");
		options.push_back("Выход");
		selectedOption = 0;
	}
	void printMenu() {
		std::cout << "Меню:\n";
		for (size_t i = 0; i < options.size(); i++) {
			std::cout << i << ") " << options[i] << "\n";
		}
		std::cout << "Выберите пункт меню: ";
		std::cin >> selectedOption;
	}
};

class Player {
public:
	std::string name; // имя игрока
	int score;
	int w_time, l_time, total_time;
	int wins;
	int losses;
	Player() : score(0), l_time(0), total_time(0), wins(0), losses(0) {}
	
	// Метод для задания имени игрока
	void setName(const std::string& new_name) {
		name = new_name;
	}
	std::string getUserName() {
		std::string userName;
		std::cout << "Введите ваше имя: ";
		std::getline(std::cin, userName);  // Чтение строки с пробелами
		return userName;
	}
	const std::string& getName() const {
		return name;
	}
};

class Settings {
public:
	int size;
	int mines;
	int difficulty;
	std::string difficultyLabel;

	Settings() : size(size), mines(mines), difficulty(0) { updateDifficultyLabel(); }

	void printSettings() {
		std::cout << "Настройки:\n";
		std::cout << "Размер поля: " << size << "\n";
		std::cout << "Количество мин: " << getMinesReference() << "\n";
		std::cout << "Уровень сложности: " + difficultyLabel << "\n";
		std::cout << "Выберите действие:\n";
	}

	void setDifficulty(int* level) {
		int choice;
		do {
			std::cout << "Выберите уровень сложности:\n";
			std::cout << "0 -> Легкий   (9x9 ячеек, 10 мин)\n";
			std::cout << "1 -> Средний  (16x16 ячеек, 40 мин)\n";
			std::cout << "2 -> Сложный  (24x24 ячеек, 99 мин)\n";
			std::cout << "3 -> Вернуться в главное меню\n";

			std::cin >> choice;

			if (choice >= 0 && choice <= 2) {
				if (choice == 0) {
					size = 9;
					mines = 10;
				}
				else if (choice == 1) {
					size = 16;
					mines = 40;
				}
				else if (choice == 2) {
					size = 24;
					mines = 99;
				}

				difficulty = choice;
				updateDifficultyLabel();
					break;
				}
				else if (choice == 3) {
					// Выход в главное меню
					*level = -1;  // Флаг для возвращения в главное меню
					break;
				}
				else {
					std::cerr << "Неверный выбор! Попробуйте снова.\n";
				}
			} while (true);
		}

		// Метод для возврата количества мин через ссылку
		int& getMinesReference() {
			return mines;
		}
private:
	void updateDifficultyLabel() {
		switch (difficulty) {
		case 0:
			difficultyLabel = "Легкий";
			break;
		case 1:
			difficultyLabel = "Средний";
			break;
		case 2:
			difficultyLabel = "Сложный";
			break;
		default:
			difficultyLabel = "Неизвестный уровень";
			break;
		}
	}
};

class Statistics {
public:
	std::string name; // имя игрока
	int wins;
	int losses;
	long totalTime;

	//Statistics //: wins(0), losses(0), totalTime(0) {}


	void saveStatistics() {
		FILE* file = fopen("statistics.txt", "w");
		if (file) {
			fprintf(file, "%d %d %ld\n", wins, losses, totalTime);
			fclose(file);
		}
	}

	void printStatistics() {
		printf("%-20s %-15s %-15s\n", "Победы:", "Поражения:", "Общее время:");
		printf("%-20d %-15d %-15ld секунд\n", wins, losses, totalTime);
	}

	void loadStatistics() {
		FILE* file = fopen("statistics.txt", "r");
		if (file) {
			fscanf(file, "%d %d %ld", &wins, &losses, &totalTime);
			fclose(file);
		}
	}
};

class Cell {
public:
	int x, y;
    bool isMine;
	bool isBorder;
	bool isOpened;
	bool isFlagged;
	int mineCount;
	// Конструктор с инициализацией полей
	Cell() : x(0), y(0), isMine(false), isBorder(false), isOpened(false), isFlagged(false), mineCount(0) {}
};

class Field {
private:
	const int EMPTY = 0; // Пустая ячейка
	const int MINE = 10; // Мина
	const int BORDER = 100; // Граница поля
	const int FLAG = 11; // Флаг
	
	Cell field[SIZE][SIZE]; // Двумерный массив ячеек
	int mask[SIZE][SIZE]; // Маска для ячеек

public:
Settings set;
	Field() {
		initMask();
		initField();
	}
	//функция открытия ячейки
	int openCell(int x, int y) {
		int result = 1;
		// Проверяем, что ячейка не помечена флагом
		if (mask[x][y] != FLAG) {
			mask[x][y] = 1; // Открываем ячейку
			// Проверяем содержимое ячейки
			if (field[x][y].isMine) {
				result = 10; // Ячейка с миной
			}
			else if (field[x][y].mineCount == 0) {
				result = 0; // Пустая ячейка
			}
			Show(); // Обновляем отображение
		}
		return result; // Возвращаем результат
	}

	//функция для определения края поля
	bool Border(int x, int y) {
		return (x < 0 || x >= set.size || y < 0 || y >= set.size || field[x][y].isBorder);
	}

	//функция инициализации
	void initVec(Cell cells[SIZE][SIZE]) {
		for (int i = 0; i < set.size; i++) {
			for (int j = 0; j < set.size; j++) {
				// Устанавливаем границы только для крайних ячеек
				cells[i][j].isBorder = (i == 0 || j == 0 || i == set.size - 1 || j == set.size - 1);
			}
		}
	}

	//функция инициализации поля
	void initField() {
		initVec(field);
	}

	void resetField() {
		// Очистка старого поля
		for (int i = 0; i < set.size; i++) {
			for (int j = 0; j < set.size; j++) {
				field[i][j].isMine = false; // Или другой индикатор пустой ячейки
			}
		}
	}

	//Маска для ячеек
	void initMask() {
		for (int i = 0; i < set.size; i++) {
			for (int j = 0; j < set.size; j++) {
				if (i == 0 || j == 0 || i == set.size - 1 || j == set.size - 1) {
					mask[i][j] = BORDER; // Устанавливаем границы
				}
				else {
					mask[i][j] = EMPTY; // Инициализация маски
				}
			}
		}
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
		for (int i = 0; i < set.size; i++) {
			for (int j = 0; j < set.size; j++) {

				if (mask[j][i] == FLAG) {
					coutColor('F', 4);
					//continue;
				}
				else if (mask[j][i] == EMPTY && !field[j][i].isOpened) {
					cout << ".";
					//continue;
				}
				else if (field[j][i].isMine == true) cout << "*";
				else if (mask[j][i] == BORDER || field[j][i].isBorder) {
					coutColor('#', 9);
				} 
				else if (field[j][i].mineCount == 0) cout << " ";
				else if (field[j][i].mineCount == 1) coutColor('1', 1);
				else if (field[j][i].mineCount == 2) coutColor('2', 2);
				else if (field[j][i].mineCount == 5) coutColor('5', 6);
				else if (field[j][i].mineCount == 3) coutColor('3', 4);
				else if (field[j][i].mineCount == 4) coutColor('4', 13);
				else if (field[j][i].mineCount == 6) coutColor('6', 8);
				else if (field[j][i].mineCount == 7) coutColor('7', 10);
				else if (field[j][i].mineCount == 8) coutColor('8', 11);
			}
			cout << endl;
		}
	}

	//функция расстановки мин
	void placeMines(int mines, int _x, int _y, int* placedMines) {
		// Проверка на допустимое количество мин
		if (mines >= (set.size - 2) * (set.size - 2)) return;

		*placedMines = 0; // Обнуляем счётчик для указателя

		for (int i = 0; i < mines;) {
			int x = rand() % (set.size - 2) + 1;
			int y = rand() % (set.size - 2) + 1;

			// Не ставим мину на первую открытую клетку
			if (_x == x && _y == y) continue;

			if (!field[x][y].isMine) {
				field[x][y].isMine = true;
				i++; // Ставим мину и увеличиваем счётчик
				(*placedMines)++; // Увеличиваем счётчик через указатель
			}
		}
	}

	//функция расстановки чисел
	void setDigits() {
		int cnt = 0;
		for (int i = 1; i < set.size - 1; i++) {
			for (int j = 1; j < set.size - 1; j++) {
				if (field[j][i].isMine)
					continue;
				if (field[j + 1][i].isMine)
					cnt++;
				if (field[j - 1][i].isMine)
					cnt++;
				if (field[j][i + 1].isMine)
					cnt++;
				if (field[j][i - 1].isMine)
					cnt++;
				if (field[j + 1][i + 1].isMine)
					cnt++;
				if (field[j - 1][i - 1].isMine)
					cnt++;
				if (field[j - 1][i + 1].isMine)
					cnt++;
				if (field[j + 1][i - 1].isMine)
					cnt++;
				field[j][i].mineCount = cnt;
				cnt = 0;
			}
		}
	}

	// функция открытия соседних пустых ячеек
	void fill(int px, int py) {
		stack<int> stk;
		stk.push(px);
		stk.push(py);
		int x = 0, y = 0;

		while (!stk.empty()) {
			y = stk.top();
			stk.pop();
			x = stk.top();
			stk.pop();

			// Проверяем границы массива перед доступом к ячейкам
			if (y + 1 < set.size && field[x][y + 1].mineCount == 0 && mask[x][y + 1] == EMPTY) {
				stk.push(x);
				stk.push(y + 1);
			}
			mask[x][y + 1] = 1;

			if (y - 1 >= 0 && field[x][y - 1].mineCount == 0 && mask[x][y - 1] == EMPTY) {
				stk.push(x);
				stk.push(y - 1);
			}
			mask[x][y - 1] = 1;

			if (x + 1 < set.size && field[x + 1][y].mineCount == 0 && mask[x + 1][y] == EMPTY) {
				stk.push(x + 1);
				stk.push(y);
			}
			mask[x + 1][y] = 1;

			if (x - 1 >= 0 && field[x - 1][y].mineCount == 0 && mask[x - 1][y] == EMPTY) {
				stk.push(x - 1);
				stk.push(y);
			}
			mask[x - 1][y] = 1;

			if (x + 1 < set.size && y + 1 < set.size && field[x + 1][y + 1].mineCount == 0 && mask[x + 1][y + 1] == EMPTY) {
				stk.push(x + 1);
				stk.push(y + 1);
			}
			mask[x + 1][y + 1] = 1;

			if (x - 1 >= 0 && y + 1 < set.size && field[x - 1][y + 1].mineCount == 0 && mask[x - 1][y + 1] == EMPTY) {
				stk.push(x - 1);
				stk.push(y + 1);
			}
			mask[x - 1][y + 1] = 1;

			if (x - 1 >= 0 && y - 1 >= 0 && field[x - 1][y - 1].mineCount == 0 && mask[x - 1][y - 1] == EMPTY) {
				stk.push(x - 1);
				stk.push(y - 1);
			}
			mask[x - 1][y - 1] = 1;

			if (x + 1 < set.size && y - 1 >= 0 && field[x + 1][y - 1].mineCount == 0 && mask[x + 1][y - 1] == EMPTY) {
				stk.push(x + 1);
				stk.push(y - 1);
			}
			mask[x + 1][y - 1] = 1;
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
			for (int x = 0; x < set.size; x++) {
				for (int y = 0; y < set.size; y++) {
					if (field[x][y].isMine && mask[x][y] == EMPTY) {
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
	void sprav() {
		cout << "Правила игры" << endl;
		cout << "Игровое поле разделено на клетки, некоторые из которых заминированы.\nДля победы вам нужно открыть все клетки, не попадая на мины.\nВ открытых клетках отображаются цифры, каждая цифра — это количество мин в соседних клетках.\nС помощью этой информации можно определить, в каких клетках содержатся мины.\nПредполагаемую клетку с миной можно пометить флажком с помощью пробела." << endl;
		system("pause");
	}
	bool firstOpen = true; // Флаг для отслеживания первой открытой ячейки
public:

	void gameOver() {
		gotoxy(40, 10);
		cout << "Вы проиграли!" << endl;
		Sleep(2000);

		system("pause");
	}

	void win() {
		gotoxy(40, 10);
		cout << "Вы победили!";
		Sleep(2000);

		system("pause");
	}

	void run() {
		Logo();

		Player player;
		Menu menu;
		Field field;
		Statistics statistics{};
		Keyboard kb;
		Cursor cs;
		//HighScores scores;

		int placedMinesUsingPointer;
		int l;
		player.setName(player.getUserName());
		field.set.setDifficulty(&l);
		while (true) {
			system("cls"); // Очистка консоли
			gotoxy(0, 0); // Убедись, что курсор в начале
			menu.printMenu();
			if (menu.selectedOption == 0) {
				system("cls"); // Очистка консоли
				field.resetField();
				field.initField();
				field.initMask();
				field.Show();
				//field.placeMines(field.set.mines, cs.getX(), cs.getY(), &placedMinesUsingPointer);
				time_t start_time = time(NULL);

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
							field.placeMines(field.set.mines, cs.getX(), cs.getY(), &placedMinesUsingPointer);
							std:cout << string(field.set.size, ' ') + "Мин размещено (указатель): " << placedMinesUsingPointer << std::endl;
							field.setDigits();
							firstOpen = false;
						}
						int res = field.openCell(cs.getX(), cs.getY());
						if (res != 1) {
							if (res == 10) {
								gameOver();
								exit = true;
								firstOpen = true;
								system("cls"); // Очистка консоли
							}
							if (res == 0) {
								field.fill(cs.getX(), cs.getY());
							}
						}
						player.total_time = time(NULL) - start_time;
						if (field.checkWin()) {
							
							win();
							exit = true; // Завершаем игру
							firstOpen = true;
							system("cls"); // Очистка консоли
						}
						break;
					}
					if (field.Border(cs.getX(), cs.getY()))
					{
						cs.undo();
					}
					cs.move();
				}
				// Обновление статистики после окончания игры
				if (player.score > 0) statistics.wins++;
				else statistics.losses++;
				statistics.totalTime += player.total_time;
				statistics.saveStatistics(); // Сохраняем статистику
				//scores.addRecord(name, player.score, player.time);
			}
			else if (menu.selectedOption == 1) {
				system("cls");
				field.set.printSettings();
				field.set.setDifficulty(&l);
			}
			else if (menu.selectedOption == 2) {
				system("cls");
				printf("Статистика игрока: %s\n", player.name.c_str());  // Жирным шрифтом заголовок
				statistics.printStatistics();
				system("pause");
			}
			else if (menu.selectedOption == 3) {
				system("cls");
				sprav();
			}
			else if (menu.selectedOption == 4) {
				statistics.saveStatistics();
				exit(0);
			}
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

// топ игроков
// усовершенствовать класс игрок
// выбор уровня в настройках игры
// исправить печать

// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <limits>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <conio.h>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>

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

class GameEntity {
public:
	GameEntity() {
		// Конструктор базового класса
		std::cout << "Инициализация прошла успешно." << std::endl;
	}
};

class Menu : public GameEntity {
private:
	std::vector<std::string> options;
public:
	int selectedOption;
	Menu() : GameEntity() {
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
		while (true) {
			std::cin >> selectedOption;
			if (std::cin.fail() || selectedOption < 0 || selectedOption >= options.size()) {
				std::cin.clear(); // Сброс состояния потока
				std::cin.ignore(10000, '\n'); // Игнорируем до 10000 символов или до новой строки
				std::cerr << "Неверный выбор! Пожалуйста, введите номер пункта от 0 до " << options.size() - 1 << ": ";
			}
			else {
				std::cin.ignore(10000, '\n'); // Игнорируем до 10000 символов или до новой строки
				break; // Верный ввод
			}
		}
	}
};

class Player : public GameEntity {
public:
	std::string name; // имя игрока
	int total_time;
	int wins;
	int losses;
	Player() : GameEntity(), name(""), total_time(0), wins(0), losses(0) {}
	
	// Метод для задания имени игрока
	void setName(const std::string& new_name) {
		name = new_name;
	}

	std::string getUserName() {
		std::string userName;
		std::cout << "Введите ваше имя: ";
		std::getline(std::cin, userName);  // Чтение строки с пробелами
		while (userName.empty() || std::all_of(userName.begin(), userName.end(), ::isspace)) {
			std::cerr << "Имя не может быть пустым или состоять только из пробелов! Пожалуйста, введите ваше имя снова: ";
			std::getline(std::cin, userName);
		}
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

	Settings() : size(9), mines(10), difficulty(0) { updateDifficultyLabel(); }

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
			if (std::cin.fail() || choice < 0 || choice > 3) {
				std::cin.clear(); // Сброс состояния потока
				std::cin.ignore(10000, '\n'); // Игнорируем до 10000 символов или до новой строки
				std::cerr << "Неверный выбор! Пожалуйста, введите номер от 0 до 3: ";
			}
			else {
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
private:
	std::string name; // имя игрока
	int wins;
	int losses;
	long totalTime;
public:
	Statistics() : wins(0), losses(0), totalTime(0), name("Игрок") {}

	// Конструктор копирования
	Statistics(const Statistics& other) {
		this->name = other.name;
		this->wins = other.wins;
		this->losses = other.losses;
		this->totalTime = other.totalTime;
	}

	// Оператор присваивания
	Statistics& operator=(const Statistics& other) {
		if (this == &other) {
			return *this; // Проверка на самоприсваивание
		}
		this->name = other.name;
		this->wins = other.wins;
		this->losses = other.losses;
		this->totalTime = other.totalTime;
		return *this;
	}

	// Перегрузка оператора сравнения
	bool operator==(const Statistics& other) const {
		return (this->wins == other.wins) && (this->losses == other.losses) && (this->totalTime == other.totalTime);
	}

	// Публичные методы для доступа к приватным полям
	void TotalTime(long num) { this->totalTime += num; }

	void saveStatistics() {
		std::ofstream outFile("statistics.txt");
		if (outFile.is_open()) {
			outFile << this->name << "\n" << this->wins << "\n" << this->losses << "\n" << this->totalTime << "\n";
			outFile.close();
		}
		else {
			std::cerr << "Не удалось открыть файл для записи.\n";
		}
	}

	void printStatistics() {
		printf("%-20s %-15s %-15s\n", "Победы:", "Поражения:", "Общее время:");
		printf("%-20d %-15d %-15ld секунд\n", this->wins, this->losses, this->totalTime);
	}

	void loadStatistics() {
		try {
			std::ifstream inFile("statistics.txt");
			if (!inFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл для чтения.");
			}

			std::getline(inFile, this->name);
			inFile >> this->wins >> this->losses >> this->totalTime;

			if (inFile.fail()) {
				throw std::runtime_error("Ошибка при чтении данных из файла.");
			}

			inFile.close();
		}
		catch (const std::exception& e) {
			std::cerr << "Произошла ошибка: " << e.what() << std::endl;
			throw; // Выбрасываем исключение дальше для обработки выше
		}
	}

	void resetStatistics(std::string Name) {
		this->wins = 0;
		this->losses = 0;
		this->totalTime = 0;
		this->name = Name; // Установим имя по умолчанию
	}

	std::string getStatName() {
		std::ifstream inFile("statistics.txt");
		if (inFile.is_open()) {
			inFile >> this->name;
			inFile.close();
		}
		return this->name;
	}

	// Получаем указатель на количество побед
  int* getWinsPointer() {
      return &wins;
  }

  // Получаем указатель на количество поражений
  int* getLossesPointer() {
      return &losses;
  }

	// Дружественная функция для вывода статистики
	friend void displayStatistics(const Statistics& stats);
};

// Дружественная функция
void displayStatistics(const Statistics& stats) {
	printf("%-20s %-15s %-15s\n", "Победы:", "Поражения:", "Общее время:");
	printf("%-20d %-15d %-15ld секунд\n", stats.wins, stats.losses, stats.totalTime);
}

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
static int objectCount;
	Field() {
		initMask();
		initField();
		objectCount++;  // Увеличиваем счетчик при создании нового объекта
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

	// Статический метод для получения количества объектов
	static int getObjectCount() {
		return objectCount;
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

	void flag(int x, int y, int* remainMines) {
		if (mask[x][y] != FLAG) { // Если флаг не установлен
			mask[x][y] = FLAG; // Устанавливаем флаг
			(*remainMines)--; // Уменьшаем количество оставшихся мин
		}
		else { // Если флаг уже установлен
			mask[x][y] = EMPTY; // Убираем флаг
			(*remainMines)++; // Увеличиваем количество оставшихся мин
		}
	}

	bool checkWin() {
			for (int x = 0; x < set.size; x++) {
				for (int y = 0; y < set.size; y++) {
					if (!field[x][y].isMine && mask[x][y] == EMPTY) {
						return false; // Если есть закрытая ячейка без мины, игрок не выиграл
					}
				}
			}
			return true; // Все ячейки без мин открыты, игрок выиграл
		}

	void gameOver() {
		// Показываем все мины
		for (int x = 0; x < set.size; x++) {
			for (int y = 0; y < set.size; y++) {
				if (field[x][y].isMine) {
					setColor(RED, WHITE);
					gotoxy(x, y);
					std::cout << "*"; // Отображаем мину
				}
			}
		}
		gotoxy(40, 10);
		std::cout << "Игра окончена! Вы проиграли.";
		system("pause");
		setColor(BLACK, WHITE); // Восстанавливаем цвет
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
	void showInstructions() {
        std::cout << "Правила игры" << std::endl;
        std::cout << "Игровое поле разделено на клетки, некоторые из которых заминированы.\n"
                     "Для победы вам нужно открыть все клетки, не попадая на мины.\n"
                     "В открытых клетках отображаются цифры, каждая цифра — это количество мин в соседних клетках.\n"
                     "С помощью этой информации можно определить, в каких клетках содержатся мины.\n"
                     "Предполагаемую клетку с миной можно пометить флажком с помощью пробела." << std::endl;
        system("pause");
    }
	std::unique_ptr<Menu> menu;
	bool firstOpen = true; // Флаг для отслеживания первой открытой ячейки
public:
	Game() {
		menu = std::make_unique<Menu>(); // Инициализируем меню
	}
	void win() {
		gotoxy(40, 10);
		cout << "Вы победили!";
 		system("pause");
		setColor(BLACK, WHITE); // Восстанавливаем цвет
	}

	void run() {

		Logo();

		Player player;
		//Menu menu;
		Field field;
		Statistics statistics, stats2;
		Keyboard kb;
		Cursor cs;
		int l;
		player.setName(player.getUserName());

		if (player.getName() == statistics.getStatName()) {
			int option;
			std::cout << "Имя совпадает с именем игрока в статистике. Вы хотите:\n";
			std::cout << "1. Восстановить статистику из файла\n";
			std::cout << "2. Создать новую статистику и начать заново\n";
			while (true) {
				std::cin >> option;
				if (std::cin.fail() || option <= 0 || option >= 3) {
					std::cin.clear(); // Сброс состояния потока
					std::cin.ignore(10000, '\n'); // Игнорируем до 10000 символов или до новой строки
					std::cerr << "Неверный выбор! Пожалуйста, введите номер пункта(1 или 2) " << ": ";
				}
				else {
					std::cin.ignore(10000, '\n'); // Игнорируем до 10000 символов или до новой строки
					break; // Верный ввод
				}
			}
			if (option == 1) {
				statistics.loadStatistics();
				stats2 = statistics; // Используется конструктор копирования
			}
			else if (option == 2) {
				statistics.resetStatistics(player.getName()); // Обнуляем статистику
			}
		}
		else if (player.getName() != statistics.getStatName()) {
			statistics.resetStatistics(player.getName());
		}
		field.set.setDifficulty(&l);

		while (true) {
			system("cls"); // Очистка консоли
			gotoxy(0, 0); // Убедись, что курсор в начале
			menu->printMenu();
			if (menu->selectedOption == 0) {
				system("cls"); // Очистка консоли
				field.resetField();
				field.initField();
				field.initMask();
				field.Show();
				int remainingMines = field.set.mines;
				int placedMinesUsingPointer = 0;
				time_t start_time = time(NULL);
				firstOpen = true; // Сбрасываем флаг

				cs.move();
				bool exit = false;

				while (!exit) {
					kb.waitKey();
					cs.save();

					gotoxy(field.set.size, 0); // Устанавливаем курсор в начало, чтобы таймер выводился там
					std::cout << std::string(field.set.size, ' ') << "Таймер: " << difftime(time(NULL), start_time) << " секунд" << std::endl;
					gotoxy(0, field.set.size);
					if (remainingMines < 10) {
						std::cout << "Осталось пометить мин: " << remainingMines << " " << std::endl; // Для однозначного числа
					}
					else {
						std::cout << "Осталось пометить мин: " << remainingMines << std::endl; // Для двузначного числа
					}
					player.total_time = difftime(time(NULL), start_time);
					
					switch (kb.getKey())
					{
					case 77: cs.incX(); break;// вправо
					case 80: cs.incY(); break;// вниз
					case 75: cs.decX(); break;// влево
					case 72: cs.decY(); break;// вверх
					case 32: field.flag(cs.getX(), cs.getY(), &remainingMines); field.Show(); 
						break; //
					case 27: // ESC для выхода в меню
						exit = true;
						break;
					//нажатие на enter
					case 13:
						if (firstOpen == true) {
							field.placeMines(field.set.mines, cs.getX(), cs.getY(), &remainingMines);
							field.setDigits();
							firstOpen = false;
						}
						
						int res = field.openCell(cs.getX(), cs.getY());
						if (res != 1) {
							if (res == 10) {
								*(statistics.getLossesPointer()) += 1;
								field.gameOver();
								exit = true;
								firstOpen = true;
								system("cls"); // Очистка консоли
							}
							if (res == 0) {
								field.fill(cs.getX(), cs.getY());
							}
						}
						if (field.checkWin()) {
							*(statistics.getWinsPointer()) += 1; // Увеличиваем количество побед на 1
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
				statistics.TotalTime(player.total_time);
				statistics.saveStatistics(); // Сохраняем статистику
			}
			else if (menu->selectedOption == 1) {
				system("cls");
				field.set.printSettings();
				field.set.setDifficulty(&l);
			}
			else if (menu->selectedOption == 2) {
				system("cls");
				printf("Статистика игрока: %s\n", player.name.c_str());  // Жирным шрифтом заголовок
				displayStatistics(statistics);
				if (statistics == stats2) { // Сравниваем статистики
					std::cout << "Статистики равны. Вы ничего не изменили в статистике!" << std::endl;
				}
				else {
					std::cout << "Статистики не равны. За игру вы изменили статистику!" << std::endl;
				}
				system("pause");
			}
			else if (menu->selectedOption == 3) {
				system("cls");
				showInstructions();
			}
			else if (menu->selectedOption == 4) {
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

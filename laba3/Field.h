#pragma once
#include "Cell.h"
#include "Settings.h"

class Field {
private:
    static const int EMPTY = 0;
    static const int MINE = 10;
    static const int BORDER = 100;
    static const int FLAG = 11;

    Cell field[SIZE][SIZE];
    int mask[SIZE][SIZE];

public:
    Settings set;
    Field();
    void initField();
    void initMask();
    int openCell(int x, int y);
    void resetField();
    void placeMines(int mines, int _x, int _y, int* placedMines);
    void setDigits();
    void fill(int px, int py);
    void flag(int x, int y, int* remainMines);
    bool checkWin();
    void gameOver();

    void Show();
};
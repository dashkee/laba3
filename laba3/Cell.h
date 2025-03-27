#pragma once
class Cell {
public:
    int x, y;
    bool isMine;
    bool isBorder;
    bool isOpened;
    bool isFlagged;
    int mineCount;

    Cell();
};
#pragma once
class Cursor {
private:
    int x, y;
    int tx, ty;

public:
    Cursor();
    void save();
    void undo();
    void incX();
    void incY();
    void decX();
    void decY();
    int getX();
    int getY();
    void move();
};
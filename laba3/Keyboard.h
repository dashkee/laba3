#pragma once
class Keyboard {
private:
    int ch;

public:
    Keyboard();
    void waitKey();
    int getKey();
};
#pragma once
#include "Field.h"
#include "Player.h"
#include "Statistics.h"
#include "Menu.h"

class Game {
private:
    void Logo();
    void showInstructions();
    bool firstOpen;

public:
    Game();
    void run();
    void win();
    void gameOver();
};
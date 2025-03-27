#pragma once
#include <string>

class Settings {
public:
    int size;
    int mines;
    int difficulty;
    std::string difficultyLabel;

    Settings();
    void printSettings();
    void setDifficulty(int* level);
    int& getMinesReference();
private:
    void updateDifficultyLabel();
};
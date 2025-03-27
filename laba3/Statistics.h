#pragma once
#include <string>

class Statistics {
public:
    std::string name; // имя игрока
    int wins;
    int losses;
    long totalTime;

    Statistics();
    void saveStatistics();
    void printStatistics();
    void loadStatistics();
};
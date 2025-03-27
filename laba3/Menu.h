#pragma once
#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::string> options;
public:
    int selectedOption;

    Menu();
    void printMenu();
};
#pragma once
#include <string>

class Player {
public:
    std::string name; // ��� ������
    int score;
    int total_time, wins, losses;

    Player();
    void setName(const std::string& new_name);
    std::string getUserName();
    const std::string& getName() const;
};
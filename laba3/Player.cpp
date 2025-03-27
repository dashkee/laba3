#include "Player.h"
#include <iostream>

Player::Player() : score(0), total_time(0), wins(0), losses(0) {}

void Player::setName(const std::string& new_name) {
    name = new_name;
}

std::string Player::getUserName() {
    std::string userName;
    std::cout << "¬ведите ваше им€: ";
    std::getline(std::cin, userName);
    return userName;
}

const std::string& Player::getName() const {
    return name;
}
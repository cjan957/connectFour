#include "Player.hpp"

// TODO: implement Player class here
Player::Player(const std::string& name) {
	this->playerName = name;
	this->winsCount = 0;
	this->scoreCount = 0;
}

std::string Player::getName() const {
	return this->playerName;
}

unsigned int Player::getScore() const {
	return this->scoreCount;
}

void Player::resetScore() {
	this->scoreCount = 0;
}

void Player::increaseScore() {
	this->scoreCount++;
}

unsigned int Player::getWins() const {
	return this->winsCount;
}

void Player::increaseWins() {
	this->winsCount++;
}

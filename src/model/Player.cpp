#include "Player.hpp"

Player::Player(std::string name) : name(name), isAlive(true), strategy(nullptr) {
    hand.reserve(6);
    for (int i = 0; i < 6; ++i){
        hand.emplace_back();
    }
}

Player::Player(std::string name, int numberOfDice) : name(name), isAlive(true), strategy(nullptr) {
    hand.reserve(numberOfDice);
    for (int i = 0; i < numberOfDice; ++i){
        hand.emplace_back();
    }
}

const std::string Player::getName() const { 
    return name; 
}

bool Player::checkAlive() const { 
    return isAlive; 
}

bool Player::isAI() const { 
    return strategy != nullptr; 
}

void Player::rollAllDice() {
    for (auto& die : hand){
        die.roll();
    }
}

void Player::loseDie(){
    if(!hand.empty()){
        hand.pop_back();
    }
    if(hand.empty()){
        isAlive = false;
    }
}

void Player::loseDice(int numberToLose){
    for(int i = 0; i < numberToLose; ++i){
        loseDie();
    }
}

const std::vector<Die>& Player::getHand() const { 
    return hand; 
}

int Player::getDiceCount() const { 
    return hand.size(); 
}

void Player::addBid(const Bid& bid) {
    bids.push_back(bid);
}

std::optional<Bid> Player::getLastBid() const {
    if (bids.empty()) return std::nullopt;
    return bids.back();
}

void Player::clearHistory() {
    bids.clear();
}

const std::vector<Bid>& Player::getFullHistory() const { 
    return bids; 
}
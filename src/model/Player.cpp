#include "Player.hpp"
#include "IStrategy.hpp"
#include "Die.hpp"

Player::~Player() = default;

Player::Player(Player&& other) noexcept = default;
Player& Player::operator=(Player&& other) noexcept = default;

Player::Player(std::string name) : name(name), alive(true), strategy(nullptr) {
    hand.reserve(6);
    for (int i = 0; i < 6; ++i){
        hand.emplace_back();
    }
}

Player::Player(std::string name, int numberOfDice) : name(name), alive(true), strategy(nullptr) {
    hand.reserve(numberOfDice);
    for (int i = 0; i < numberOfDice; ++i){
        hand.emplace_back();
    }
}

const std::string Player::getName() const { 
    return name; 
}

bool Player::isAlive() const { 
    return alive; 
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
        alive = false;
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

bool Player::operator==(const Player& other) const = default;
bool Player::operator!=(const Player& other) const = default;
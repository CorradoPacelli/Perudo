#include "GameModel.hpp"
#include "RollingState.hpp"
#include "IGameState.hpp"

GameModel::~GameModel() = default;

void GameModel::startGame() {
    // TODO: check for other things to do when a game start
    gameOver = false;
    currentPlayerIndex = 0;
    changeState(std::make_unique<RollingState>());
}

bool GameModel::isGameOver() {
    return gameOver;
}

void GameModel::stopGame() {
    gameOver = true;
}

void GameModel::addPlayer(const Player player) { 
    players.emplace_back(player); 
}

void GameModel::nextPlayer() { 
    int previousPlayer = currentPlayerIndex;
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players.at(currentPlayerIndex).isAlive() && currentPlayerIndex != previousPlayer);

    if (currentPlayerIndex == previousPlayer){
        // TODO: 
        // You should never be in the situation where there is only one player playing. 
        // The resoloution of the entire game should have already happen, let's throw for now
        throw;
    }
}

void GameModel::changeState(std::unique_ptr<IGameState> newState) {
    currentState = std::move(newState);
    currentState->onEnter(*this);
}

Player& GameModel::getCurrentPlayer() { 
    return players.at(currentPlayerIndex); 
}

const Player& GameModel::getCurrentPlayer() const { 
    return getCurrentPlayer(); 
}

const Player& GameModel::getPreviousAlivePlayer() const { 
    return getPreviousAlivePlayer(); 
}

Player& GameModel::getPreviousAlivePlayer() { 
    int previousPlayer = currentPlayerIndex;
    do {
        --previousPlayer;
        if(previousPlayer < 0) {
            previousPlayer = players.size() -1;
        }
    } while (!players.at(previousPlayer).isAlive() && currentPlayerIndex != previousPlayer);

    if (currentPlayerIndex == previousPlayer){
        // TODO: 
        // You should never be in the situation where there is only one player playing. 
        // The resoloution of the entire game should have already happen, let's throw for now
        throw;
    }
    return players.at(previousPlayer);
}

bool GameModel::isOnlyOnePlayerAlive() const {
    int nextPlayerIndex = currentPlayerIndex;
    int numberOfPlayerAlive = 0;
    do {
        if (players.at(nextPlayerIndex).isAlive()) ++numberOfPlayerAlive;
        if (numberOfPlayerAlive > 1) return false;
        nextPlayerIndex = (nextPlayerIndex + 1) % players.size();
    } while (nextPlayerIndex != currentPlayerIndex);

    return true;
}

std::vector<Player>& GameModel::getPlayers() { 
    return getPlayers(); 
}

const std::vector<Player>& GameModel::getPlayers() const { 
    return players; 
}

std::optional<Bid> GameModel::getLastBid() const { 
    return players.at(currentPlayerIndex).getLastBid(); 
}

void GameModel::render(IGameView& view) const {
    if (currentState) {
        currentState->render(*this, view);
    }
}

bool GameModel::requiresAction() const {
    return currentState ? currentState->requiresAction() : false;
}

void GameModel::handleAction(const IAction& action) {
    if (currentState) {
        currentState->handleAction(*this, action);
    }
}

void GameModel::step() {
    if (currentState) {
        currentState->step(*this);
    }
}
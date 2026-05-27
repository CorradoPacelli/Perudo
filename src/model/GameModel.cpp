#include "GameModel.hpp"
#include "RollingState.hpp"
#include "IGameState.hpp"
#include "IAction.hpp"

GameModel::GameModel() = default;
GameModel::~GameModel() = default;

GameModel::GameModel(GameModel&& other) noexcept = default;
GameModel& GameModel::operator=(GameModel&& other) noexcept = default;

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

void GameModel::nextPlayer() { 
    int previousPlayer = currentPlayerIndex;
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players.at(currentPlayerIndex).isAlive() && currentPlayerIndex != previousPlayer);

    if (currentPlayerIndex == previousPlayer){
        // TODO: 
        // You should never be in the situation where there is only one player playing. 
        // The resoloution of the entire game should have already happen, let's throw for now
         throw std::logic_error("Only one player is alive, cannot get next player.");
    }
}

void GameModel::changeState(std::unique_ptr<IGameState> newState) {
    currentState = std::move(newState);
    currentState->onEnter(*this);
}

Player& GameModel::getCurrentPlayer() { 
    return const_cast<Player&>(static_cast<const GameModel&>(*this).getCurrentPlayer()); 
}

const Player& GameModel::getCurrentPlayer() const { 
    return players.at(currentPlayerIndex); 
}

const Player& GameModel::getPreviousAlivePlayer() const { 
    int previousPlayer = currentPlayerIndex;
    do {
        --previousPlayer;
        if(previousPlayer < 0) {
            previousPlayer = players.size() -1;
        }
    } while (!players.at(previousPlayer).isAlive() && currentPlayerIndex != previousPlayer);

    if (currentPlayerIndex == previousPlayer){
        // This should be unreachable if isOnlyOnePlayerAlive() is checked before state transitions.
        throw std::logic_error("Only one player is alive, cannot get previous player.");
    }
    return players.at(previousPlayer);
}

Player& GameModel::getPreviousAlivePlayer() { 
    // Safely call the const version and cast away the constness of the return type
    return const_cast<Player&>(static_cast<const GameModel&>(*this).getPreviousAlivePlayer());
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
    return players; 
}

const std::vector<Player>& GameModel::getPlayers() const { 
    return players; 
}

std::optional<Bid> GameModel::getLastBid() const { 
    return getPreviousAlivePlayer().getLastBid(); 
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
    // This is the authoritative check. The Model enforces its own rules.
    if (action.getPlayerId() != this->currentPlayerIndex) {
        throw std::invalid_argument("It is not your turn to act.");
    }

    if (currentState) {
        currentState->handleAction(*this, action);
    }
}

void GameModel::step() {
    if (currentState) {
        currentState->step(*this);
    }
}
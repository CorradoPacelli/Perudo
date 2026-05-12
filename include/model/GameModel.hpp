#pragma once

#include <vector>
#include <memory>

#include "Player.hpp"
#include "IGameState.hpp"

class IGameView;
class IAction;

class GameModel {
    // Only the base interface is a friend. It delegates state changes to derived classes.
    friend class IGameState;

private:
    std::vector<Player> players;
    int currentPlayerIndex = 0;
    bool gameOver = false;
    std::unique_ptr<IGameState> currentState;

    void changeState(std::unique_ptr<IGameState> newState);

public:
    void startGame();
    bool isGameOver();
    void stopGame();
    
    void addPlayer(const Player player);
    void nextPlayer();
    Player& getCurrentPlayer();
    Player& getPreviousAlivePlayer();
    bool isOnlyOnePlayerAlive() const;
    std::vector<Player>& getPlayers();
    std::optional<Bid> getLastBid() const;
    
    void render(IGameView& view) const;
    void handleAction(const IAction& action);
};
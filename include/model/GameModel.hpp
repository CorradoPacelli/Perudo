#pragma once

#include <vector>
#include <memory>

#include "Player.hpp"
#include "IGameState.hpp"

class GameModel {
    // Only the base interface is a friend. It delegates state changes to derived classes.
    friend class IGameState;

private:
    std::vector<Player> players;
    int currentPlayerIndex = 0;
    std::unique_ptr<IGameState> currentState;

    void changeState(std::unique_ptr<IGameState> newState);
public:
    void addPlayer(const Player player);
    void nextPlayer();
    Player& getCurrentPlayer();
    Player& getPreviousAlivePlayer();
    bool isOnlyOnePlayerAlive() const;
    std::vector<Player>& getPlayers();
    std::optional<Bid> getLastBid() const;
};
#pragma once

#include <vector>
#include <memory>

#include "Player.hpp"
#include "IGameState.hpp"

class GameModel {
private:
    std::vector<Player> players;
    int currentPlayerIndex = 0;
    std::unique_ptr<IGameState> currentState;

public:
    void changeState(std::unique_ptr<IGameState> newState);
    void addPlayer(const Player player);
    void nextPlayer();
    Player& getCurrentPlayer();
    Player& getPreviousPlayer();
    std::vector<Player>& getPlayers();
    std::optional<Bid> getLastBid() const;
};
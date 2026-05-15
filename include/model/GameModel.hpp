#pragma once

#include <vector>
#include <memory>

#include "Player.hpp"

class IGameView;
class IAction;
class IGameState;

class GameModel {
    // Only the base interface is a friend. It delegates state changes to derived classes.
    friend class IGameState;

private:
    std::vector<Player> players;
    int currentPlayerIndex = 0;
    bool gameOver = false;
    std::unique_ptr<IGameState> currentState;

    // TODO: maybe I can get rid of this function and the friend class, step function in IGameState could return the new state...
    void changeState(std::unique_ptr<IGameState> newState);

public:
    GameModel();
    ~GameModel();

    GameModel(const GameModel& other) = delete;
    GameModel& operator=(const GameModel& other) = delete;
    GameModel(GameModel&& other) noexcept;
    GameModel& operator=(GameModel&& other) noexcept;

    void startGame();
    bool isGameOver();
    void stopGame();
    
    void nextPlayer();
    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;
    Player& getPreviousAlivePlayer();
    const Player& getPreviousAlivePlayer() const;
    bool isOnlyOnePlayerAlive() const;
    std::vector<Player>& getPlayers();
    const std::vector<Player>& getPlayers() const;
    std::optional<Bid> getLastBid() const;
    
    bool requiresAction() const;
    void handleAction(const IAction& action);
    void step();
    void render(IGameView& view) const;

    template<typename... Args>
    void addPlayer(Args&&... args) {
        players.emplace_back(std::forward<Args>(args)...);
    }
};
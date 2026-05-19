#pragma once

#include "GameModel.hpp"
#include "Player.hpp"
#include "Die.hpp"
#include <vector>

class GameModelTestAccessor {
public:
    static IGameState* getCurrentState(const GameModel& model) {
        return model.currentState.get();
    }

    static void setPlayerDice(Player& player, const std::vector<short int>& faces) {
        player.hand.clear();
        for (short int face : faces) {
            Die d;
            d.face = face;
            player.hand.push_back(d);
        }
    }
};
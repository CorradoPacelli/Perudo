#pragma once

#include "GameModel.hpp"

class GameModelTestAccessor {
public:
    static IGameState* getCurrentState(const GameModel& model) {
        return model.currentState.get();
    }
};
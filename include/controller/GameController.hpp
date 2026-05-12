#pragma once

#include "GameModel.hpp"
#include "IGameView.hpp"
#include <memory>

class GameController {
private:
    GameModel& model;
    IGameView& view;

public:
    GameController(GameModel& m, IGameView& v);

    void run();
};
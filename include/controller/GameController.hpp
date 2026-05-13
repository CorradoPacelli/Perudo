#pragma once

#include <memory>

class GameModel;
class IGameView;

class GameController {
private:
    GameModel& model;
    IGameView& view;

public:
    GameController(GameModel& m, IGameView& v);

    void run();
};
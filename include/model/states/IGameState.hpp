#pragma once

#include <string>
#include <memory>

#include "IAction.hpp"

class GameModel;
class IGameView;

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void onEnter(GameModel& context) = 0;

    virtual void handleAction(GameModel& context, const IAction& action) = 0;

    virtual void render(const GameModel& context, IGameView& view) const = 0;
};
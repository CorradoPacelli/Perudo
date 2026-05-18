#pragma once

#include "IGameState.hpp"

class RollingState : public IGameState {
public:
    RollingState() = default;
    virtual ~RollingState() = default;

    void onEnter(GameModel& context) override;

    void step(GameModel& context) override;

    void render(const GameModel& context, IGameView& view) const override;
};
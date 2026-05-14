#pragma once

#include "IGameState.hpp"

class RollingState : public IGameState {
public:
    RollingState() = default;
    virtual ~RollingState() = default;

    void onEnter(GameModel& context) override;

    bool requiresAction() const override;

    void handleAction(GameModel& context, const IAction& action) override;

    void step(GameModel& context) override;

    void render(const GameModel& context, IGameView& view) const override;
};
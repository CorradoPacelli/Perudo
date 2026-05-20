#pragma once

#include <optional>

#include "IGameState.hpp"
#include "IAction.hpp"
#include "Bid.hpp"

class ResolutionState : public IGameState {
public:
    ResolutionState(ActionType triggerAction);
    virtual ~ResolutionState() = default;

    void onEnter(GameModel& context) override;

    void render(const GameModel& context, IGameView& view) const override;

    void step(GameModel& context) override;

private:
    ActionType triggerAction;

    std::optional<Bid> resolvedBid;
    int actualDiceQuantity = 0;
    bool callerWon = false;

    int countDiceOfFace(const GameModel& context, short int face) const;
};
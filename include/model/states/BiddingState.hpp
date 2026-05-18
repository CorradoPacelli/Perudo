#pragma once

#include "IGameState.hpp"

class BiddingState : public IGameState {
public:
    BiddingState() = default;
    virtual ~BiddingState() = default;

    void onEnter(GameModel& context) override;

    bool requiresAction() const override;

    /**
     * @brief This method handles the bids and change the state accordingly
     */
    void handleAction(GameModel& context, const IAction& action) override;

    void render(const GameModel& context, IGameView& view) const override;
};
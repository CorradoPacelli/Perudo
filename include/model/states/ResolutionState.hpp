#pragma once

#include "IGameState.hpp"

class ResolutionState : public IGameState {
public:
    ResolutionState() = default;
    virtual ~ResolutionState() = default;


    void onEnter(GameModel& context) override;

    void handleAction(GameModel& context, const IAction& action) override;

    void render(const GameModel& context, IGameView& view) const override;

private:
    
    /** 
     * @brief This is an helper function to check if the last bid was:
     * a lie            -> return -1
     * perfect          -> return 0 (exactly the number of dice)
     * fair             -> return 1
     */
    int checkLastBid(GameModel& context);
};
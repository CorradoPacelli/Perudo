#pragma once

#include "IGameState.hpp"
#include "IAction.hpp"

class ResolutionState : public IGameState {
public:
    ResolutionState(ActionType triggerAction);
    virtual ~ResolutionState() = default;

    void onEnter(GameModel& context) override;

    void render(const GameModel& context, IGameView& view) const override;

    void step(GameModel& context) override;

private:
    ActionType triggerAction;
    std::string resultMessage;
    
    /** 
     * @brief This is an helper function to check if the last bid was:
     * a lie            -> return -1
     * perfect          -> return 0 (exactly the number of dice)
     * fair             -> return 1
     */
    int checkLastBid(GameModel& context);
};
#include "RollingState.hpp"
#include "BiddingState.hpp"
#include "GameModel.hpp"

void RollingState::onEnter(GameModel& context) {
    for (auto& p : context.getPlayers()) {
        if (p.isAlive()) {
            p.clearHistory();
            p.rollAllDice();
        }
    }

    context.changeState(std::make_unique<BiddingState>());
}

void RollingState::handleAction(GameModel& context, const IAction& action) {
    // nothing to do in this state, there is no input
}
#include "RollingState.hpp"
#include "BiddingState.hpp"
#include "IGameView.hpp"
#include "GameModel.hpp"

void RollingState::onEnter(GameModel& context) {
    for (auto& p : context.getPlayers()) {
        if (p.isAlive()) {
            p.clearHistory();
            p.rollAllDice();
        }
    }

    requestStateChange(context, std::make_unique<BiddingState>());
}

void RollingState::handleAction(GameModel& context, const IAction& action) {
    // nothing to do in this state, there is no input
}

void RollingState::render(const GameModel& context, IGameView& view) const{
    view.displayMessage("\n--- ROLLING DICE FOR ALL PLAYERS ---\n");
}
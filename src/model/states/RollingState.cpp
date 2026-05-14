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
}

bool RollingState::requiresAction() const {
    return false;
}

void RollingState::handleAction(GameModel& context, const IAction& action) {
    // nothing to do in this state, there is no input
}

void RollingState::step(GameModel& context) {
    requestStateChange(context, std::make_unique<BiddingState>());
}

void RollingState::render(const GameModel& context, IGameView& view) const{
    view.displayMessage("\n--- ROLLING DICE FOR ALL PLAYERS ---\nPress Enter to look at your hand...");
}
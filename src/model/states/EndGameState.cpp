#include <stdexcept>

#include "EndGameState.hpp"
#include "GameModel.hpp"
#include "RollingState.hpp"
#include "IAction.hpp"
#include "IGameView.hpp"

void EndGameState::onEnter(GameModel& context) {
    if (!context.isOnlyOnePlayerAlive()) throw std::logic_error("More than one player alive in EndGameState"); //this should not be possible 
    for (const auto& player : context.getPlayers()) {
        if (player.isAlive()) {
            std::string winnerName = player.getName();
            // TODO: Do something with this info, maybe we have to create a new attribute for the winning Player in the GameModel
            break;
        }
    }
}

bool EndGameState::requiresAction() const {
    return true;
}

void EndGameState::handleAction(GameModel& context, const IAction& action) {
    if (action.getType() == ActionType::RESET) {
        //context.resetGame(); TODO: reset Game
        requestStateChange(context, std::make_unique<RollingState>());
    } else if (action.getType() == ActionType::EXIT) {
        context.stopGame();
    } else {
        // TODO: Here we should never accept any other action
    }
}


void EndGameState::render(const GameModel& context, IGameView& view) const {
    std::string msg = "\n======================\n";
    msg += "GAME OVER!\n";
    msg += "======================\n";
    for (const auto& player : context.getPlayers()) {
        if (player.isAlive()) {
            msg += "The winner is: " + player.getName() + "!!!\n\n";
            break;
        }
    }
    msg += "Type 'reset' to start a new game; type 'exit' to quit";
    view.displayMessage(msg);
}
#include "EndGameState.hpp"
#include "GameModel.hpp"
#include "RollingState.hpp"

void EndGameState::onEnter(GameModel& context) {
    if (!context.isOnlyOnePlayerAlive()) throw; //this should not be possible 
    for (const auto& player : context.getPlayers()) {
        if (player.isAlive()) {
            std::string winnerName = player.getName();
            // TODO: Do something with this info, maybe we have to create a new attribute for the winning Player in the GameModel
            break;
        }
    }
}

void EndGameState::handleAction(GameModel& context, const IAction& action) {
    if (action.getType() == ActionType::RESET) {
        //context.resetGame(); TODO: reset Game
        context.changeState(std::make_unique<RollingState>());
    } else if (action.getType() == ActionType::EXIT) {
        // Exit the application
    } else {
        // Here we should never accept any other action
    }
}
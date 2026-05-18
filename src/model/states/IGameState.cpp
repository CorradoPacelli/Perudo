#include <stdexcept>

#include "IGameState.hpp"
#include "GameModel.hpp"

void IGameState::step(GameModel& context) {
    throw std::logic_error("step was called on a state that does require an action.");
}

bool IGameState::requiresAction() const {
    return false;
}

void IGameState::handleAction(GameModel& context, const IAction& action) {
    throw std::logic_error("handleAction was called on a state that does not support or require actions.");
}

void IGameState::requestStateChange(GameModel& context, std::unique_ptr<IGameState> newState) {
    // Since IGameState is a friend of GameModel, it has access to the private changeState method
    context.changeState(std::move(newState));
}
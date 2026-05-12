#include "IGameState.hpp"
#include "GameModel.hpp"

void IGameState::requestStateChange(GameModel& context, std::unique_ptr<IGameState> newState) {
    // Since IGameState is a friend of GameModel, it has access to the private changeState method
    context.changeState(std::move(newState));
}
#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "EndGameState.hpp"
#include "ResetAction.hpp"
#include "ExitAction.hpp"
#include "RollingState.hpp"
#include "GameModelTestAccessor.hpp"


class EndGameStateTest : public ::testing::Test {
protected:
    GameModel model;
};

TEST_F(EndGameStateTest, StepIsCorretlyDisabled) {
    EndGameState endState;
    EXPECT_THROW(endState.step(model), std::logic_error);
}

TEST_F(EndGameStateTest, OnEnterThrowsIfMultiplePlayersAlive) {
    model.addPlayer(Player("Saitama", 5));
    model.addPlayer(Player("Genos", 5));
    
    EndGameState endState;
    // we can not enter the EndGameState with 2 alive players
    EXPECT_THROW(endState.onEnter(model), std::logic_error);
}

TEST_F(EndGameStateTest, OnEnterSucceedsWithOnePlayerAlive) {
    model.addPlayer(Player("Saitama", 5));
    
    Player genos("Genos", 5);
    genos.loseDice(5);
    model.addPlayer(std::move(genos));
    
    EndGameState endState;
    EXPECT_NO_THROW(endState.onEnter(model));
}

TEST_F(EndGameStateTest, HandleActionTransitToNewGame) {
    EndGameState endState;

    EXPECT_TRUE(endState.requiresAction());
    EXPECT_NO_THROW(endState.handleAction(model, ResetAction()));
    EXPECT_NE(dynamic_cast<RollingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(EndGameStateTest, HandleActionStopGameWithMoreThenOnePlayerAlive) {
    EndGameState endState;

    EXPECT_NO_THROW(endState.handleAction(model, ExitAction()));
    EXPECT_TRUE(model.isGameOver());
}
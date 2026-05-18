#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "ResolutionState.hpp"
#include "IAction.hpp"

class ResolutionStateTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
        model.addPlayer(Player("King", 5));
    }
};

TEST_F(ResolutionStateTest, RequiresActionReturnsFalse) {
    ResolutionState state(ActionType::DUDO);
    
    EXPECT_FALSE(state.requiresAction());
}


TEST_F(ResolutionStateTest, OnEnterThrowsIfNoPreviousBid) {
    // The GameModel starts with empty bid histories. 
    // Attempting to resolve a round without a previous bid should trigger our logic_error.
    ResolutionState state(ActionType::DUDO);
    EXPECT_THROW(state.onEnter(model), std::logic_error);
}
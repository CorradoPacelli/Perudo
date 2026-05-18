#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "RollingState.hpp"
#include "BiddingState.hpp"
#include "DudoAction.hpp"
#include "GameModelTestAccessor.hpp"


class RollingStateTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
        model.addPlayer(Player("King", 5));
    }
};

TEST_F(RollingStateTest, RequiresActionReturnsFalse) {
    RollingState state;
    
    EXPECT_FALSE(state.requiresAction());
}

TEST_F(RollingStateTest, HandleActionThrows) {
    RollingState state;

    EXPECT_THROW(state.handleAction(model, DudoAction()), std::logic_error);
}

TEST_F(RollingStateTest, OnEnterRollsDiceAndClearsHistory) {
    RollingState state;
    
    model.getCurrentPlayer().addBid(Bid(2, 3));
    ASSERT_TRUE(model.getCurrentPlayer().getLastBid().has_value());

    EXPECT_NO_THROW(state.onEnter(model));
    
    EXPECT_FALSE(model.getCurrentPlayer().getLastBid().has_value());

    for (const auto& player : model.getPlayers()) {
        if (player.isAlive()) {
            EXPECT_EQ(player.getDiceCount(), 5);
        }
    }
}

TEST_F(RollingStateTest, StepTransitions) {

    RollingState state;
    state.step(model);
    
    EXPECT_NE(dynamic_cast<BiddingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}
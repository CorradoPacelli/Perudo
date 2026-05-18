#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "BiddingState.hpp"

class BiddingStateTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
        model.addPlayer(Player("King", 5));
    }
};

TEST_F(BiddingStateTest, OnEnterDoesNotThrow) {
    BiddingState state;
    
    EXPECT_NO_THROW(state.onEnter(model));
}

TEST_F(BiddingStateTest, StepThrow) {
    BiddingState state;

    EXPECT_THROW(state.step(model), std::logic_error);
}
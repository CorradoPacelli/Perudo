#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "BiddingState.hpp"
#include "RollingState.hpp"
#include "ResolutionState.hpp"
#include "EndGameState.hpp"
#include "BidAction.hpp"
#include "DudoAction.hpp"
#include "ExactlyAction.hpp"
#include "ExitAction.hpp"
#include "GameModelTestAccessor.hpp"

class BiddingStateTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
        model.addPlayer(Player("King", 5));
    }
};

TEST_F(BiddingStateTest, StepIsCorretlyDisabled) {
    BiddingState endState;
    EXPECT_THROW(endState.step(model), std::logic_error);
}

TEST_F(BiddingStateTest, OnEnterDoesNotThrow) {
    BiddingState state;
    
    EXPECT_NO_THROW(state.onEnter(model));
}

TEST_F(BiddingStateTest, HandleDudoActionTransitionsToResolutionState) {
    BiddingState state;
    DudoAction dudo;

    // Can not say dudo as first action
    EXPECT_THROW(state.handleAction(model, dudo), std::invalid_argument); 

    model.getPreviousAlivePlayer().addBid(Bid(1, 1));
    state.handleAction(model, dudo);

    EXPECT_NE(dynamic_cast<ResolutionState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(BiddingStateTest, HandleExactlyActionTransitionsToResolutionState) {
    BiddingState state;
    ExactlyAction exactly;

    // Can not say exactly as first action
    EXPECT_THROW(state.handleAction(model, exactly), std::invalid_argument); 

    model.getPreviousAlivePlayer().addBid(Bid(1, 1));
    state.handleAction(model, exactly);

    EXPECT_NE(dynamic_cast<ResolutionState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(BiddingStateTest, HandleFirstValidBidAction) {
    BiddingState state;
    BidAction firstBid(Bid(3, 4));

    model.startGame();
    EXPECT_NE(dynamic_cast<RollingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
    model.step();
    
    ASSERT_EQ(model.getCurrentPlayer().getName(), "Saitama");
    ASSERT_FALSE(model.getLastBid().has_value());

    state.handleAction(model, firstBid);

    // After Saitama bids, the current player is Genos, and the previous is Saitama
    EXPECT_EQ(model.getPreviousAlivePlayer().getName(), "Saitama");
    EXPECT_EQ(model.getPreviousAlivePlayer().getLastBid().value(), firstBid.getBid());
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Genos");

    EXPECT_NE(dynamic_cast<BiddingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(BiddingStateTest, HandleSubsequentValidBidAction) {
    BiddingState state;

    model.startGame();
    EXPECT_NE(dynamic_cast<RollingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
    model.step();

    // The previous player (King) makes a bid first.
    model.getPlayers()[2].addBid(Bid(2, 2));
    ASSERT_EQ(model.getCurrentPlayer().getName(), "Saitama");
    
    BidAction higherBid(Bid(3, 2));
    state.handleAction(model, higherBid);

    // After Saitama bids, the current player is Genos, and the previous is Saitama
    EXPECT_EQ(model.getPreviousAlivePlayer().getName(), "Saitama");
    EXPECT_EQ(model.getPreviousAlivePlayer().getLastBid().value(), higherBid.getBid());
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Genos");

    EXPECT_NE(dynamic_cast<BiddingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(BiddingStateTest, HandleInvalidBidActionThrows) {
    BiddingState state;

    model.startGame();
    EXPECT_NE(dynamic_cast<RollingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
    model.step();

    // The previous player (King) makes a bid first.
    model.getPlayers()[2].addBid(Bid(3, 3));
    
    // Saitama tries to make a lower bid
    BidAction lowerBid(Bid(2, 6));
    EXPECT_THROW(state.handleAction(model, lowerBid), std::invalid_argument);

    // Saitama tries to make an equal bid
    BidAction equalBid(Bid(3, 3));
    EXPECT_THROW(state.handleAction(model, equalBid), std::invalid_argument);

    EXPECT_EQ(model.getCurrentPlayer().getName(), "Saitama");

    EXPECT_NE(dynamic_cast<BiddingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

TEST_F(BiddingStateTest, HandleExitActionStopsGameAndTransitions) {
    BiddingState state;
    ExitAction exit;

    // TODO: fix this, we throw because there are more than 1 player plaing, but this should be ok
    EXPECT_THROW(state.handleAction(model, exit), std::logic_error);

    EXPECT_TRUE(model.isGameOver());
    EXPECT_NE(dynamic_cast<EndGameState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}
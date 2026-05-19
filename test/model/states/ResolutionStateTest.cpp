#include <gtest/gtest.h>
#include <stdexcept>

#include "GameModel.hpp"
#include "Player.hpp"
#include "ResolutionState.hpp"
#include "IAction.hpp"
#include "DudoAction.hpp"
#include "GameModelTestAccessor.hpp"

class ResolutionStateTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
        model.addPlayer(Player("King", 5));
    }
};


TEST_F(ResolutionStateTest, HandleActionIsCorretlyDisabled) {
    ResolutionState state(ActionType::DUDO);
    
    EXPECT_FALSE(state.requiresAction());
    EXPECT_THROW(state.handleAction(model, DudoAction()), std::logic_error);
}


TEST_F(ResolutionStateTest, OnEnterThrowsIfNoPreviousBid) {
    // The GameModel starts with empty bid histories. 
    // Attempting to resolve a round without a previous bid should trigger our logic_error.
    ResolutionState state(ActionType::DUDO);
    EXPECT_THROW(state.onEnter(model), std::logic_error);
}

TEST_F(ResolutionStateTest, OnEnterWithCorrectDudo) {
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[0], {1, 2, 3, 4, 5}); // Saitama 
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[1], {1, 2, 3, 4, 5}); // Genos
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[2], {1, 2, 3, 4, 5}); // King

    // King says 4 dice of face 3 (wrong statements). King is wrong and should lose a die.
    model.getPreviousAlivePlayer().addBid(Bid(4, 3));

    ResolutionState state(ActionType::DUDO);
    EXPECT_NO_THROW(state.onEnter(model));
    
    // King was wrong, so he loses a die.
    EXPECT_EQ(model.getPreviousAlivePlayer().getDiceCount(), 4);
    EXPECT_EQ(model.getCurrentPlayer().getDiceCount(), 5);
}

TEST_F(ResolutionStateTest, OnEnterWithWrongDudo) {
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[0], {1, 2, 3, 4, 5}); // Saitama 
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[1], {1, 2, 3, 4, 5}); // Genos
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[2], {1, 2, 3, 4, 5}); // King

    // King says 2 dice of face 3 (correct statements)
    model.getPreviousAlivePlayer().addBid(Bid(2, 3));

    ResolutionState state(ActionType::DUDO);
    EXPECT_NO_THROW(state.onEnter(model));
    
    // King is right Saitama should lose a die
    EXPECT_EQ(model.getPreviousAlivePlayer().getDiceCount(), 5);
    EXPECT_EQ(model.getCurrentPlayer().getDiceCount(), 4);
}

TEST_F(ResolutionStateTest, OnEnterWithCorrectExactly) {
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[0], {1, 2, 3, 4, 5}); // Saitama (current)
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[1], {1, 2, 3, 4, 5}); // Genos
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[2], {1, 2, 3, 4, 5}); // King (previous)

    // King says 3 dice of face 3 (correct statements) Saitama calls exactly and is correct.
    model.getPreviousAlivePlayer().addBid(Bid(3, 3));

    ResolutionState state(ActionType::EXACTLY);
    EXPECT_NO_THROW(state.onEnter(model));

    // Saitama is correct, everyone else loses a die
    EXPECT_EQ(model.getPlayers()[0].getDiceCount(), 5); // Saitama
    EXPECT_EQ(model.getPlayers()[1].getDiceCount(), 4); // Genos
    EXPECT_EQ(model.getPlayers()[2].getDiceCount(), 4); // King
}

TEST_F(ResolutionStateTest, OnEnterWithWrongExactly) {
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[0], {1, 2, 3, 4, 5}); // Saitama (current)
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[1], {1, 2, 3, 4, 5}); // Genos
    GameModelTestAccessor::setPlayerDice(model.getPlayers()[2], {1, 2, 3, 4, 5}); // King (previous)

    // King says 3 dice of face 3 (correct statements) Saitama calls exactly and is correct.
    model.getPreviousAlivePlayer().addBid(Bid(4, 3));

    ResolutionState state(ActionType::EXACTLY);
    EXPECT_NO_THROW(state.onEnter(model));

    // Saitama is wrong, he loses 2 dice
    EXPECT_EQ(model.getPlayers()[0].getDiceCount(), 3); // Saitama
    EXPECT_EQ(model.getPlayers()[1].getDiceCount(), 5); // Genos
    EXPECT_EQ(model.getPlayers()[2].getDiceCount(), 5); // King
}
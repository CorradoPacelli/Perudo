#include <gtest/gtest.h>

#include "GameModel.hpp"
#include "Player.hpp"
#include "RollingState.hpp"
#include "GameModelTestAccessor.hpp"

// Test fixture for GameModel tests
class GameModelTest : public ::testing::Test {
protected:
    GameModel model;

    void SetUp() override {
        // Setup code that runs before each test to establish a baseline state
        model.addPlayer(Player("Saitama"));
        model.addPlayer(Player("King"));
        model.addPlayer(Player("Tornado of Terror"));
    }
    
    void TearDown() override {
        // Cleanup code that runs after each test
    }
};

TEST_F(GameModelTest, PlayersAddedCorrectly) {
    auto& players = model.getPlayers();
    ASSERT_EQ(players.size(), 3);
    EXPECT_EQ(players[0].getName(), "Saitama");
    EXPECT_EQ(players[1].getName(), "King");
    EXPECT_EQ(players[2].getName(), "Tornado of Terror");
}

TEST_F(GameModelTest, NextPlayerCyclesThroughAllAlivePlayers) {
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Saitama");

    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "King");

    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Tornado of Terror");

    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Saitama");
}

TEST_F(GameModelTest, PreviousAlivePlayerReturnsCorrectly) {
    // Initial player is Saitama (index 0), so previous is Tornado (index 2)
    EXPECT_EQ(model.getPreviousAlivePlayer().getName(), "Tornado of Terror");
}

TEST_F(GameModelTest, SkipsDeadPlayersWhenCycling) {
    model.getCurrentPlayer().loseDice(120); // saitama is dead

    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "King");
    
    EXPECT_EQ(model.getPreviousAlivePlayer().getName(), "Tornado of Terror");
}

TEST_F(GameModelTest, IsOnlyOnePlayerAliveUpdatesCorrectly) {
    EXPECT_FALSE(model.isOnlyOnePlayerAlive());
    model.getCurrentPlayer().loseDice(120); // saitama is gone!
    model.nextPlayer();
    model.getCurrentPlayer().loseDice(8); // king is gone!!
    EXPECT_TRUE(model.isOnlyOnePlayerAlive());
}

TEST_F(GameModelTest, Transitions) {
    model.startGame();
    EXPECT_NE(dynamic_cast<RollingState*>(GameModelTestAccessor::getCurrentState(model)), nullptr);
}

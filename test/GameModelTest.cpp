#include <gtest/gtest.h>
#include "GameModel.hpp"
#include "Player.hpp"

// Test fixture for GameModel tests
class GameModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};

// Test adding players to the game model
TEST_F(GameModelTest, AddPlayer) {
    GameModel model;
    Player player1("Alice");
    Player player2("Bob");

    model.addPlayer(player1);
    model.addPlayer(player2);

    auto& players = model.getPlayers();
    ASSERT_EQ(players.size(), 2);
    EXPECT_EQ(players[0].getName(), "Alice");
    EXPECT_EQ(players[1].getName(), "Bob");
}

// Test getting current player
TEST_F(GameModelTest, GetCurrentPlayer) {
    GameModel model;
    Player player1("Alice");
    Player player2("Bob");

    model.addPlayer(player1);
    model.addPlayer(player2);

    Player& current = model.getCurrentPlayer();
    EXPECT_EQ(current.getName(), "Alice");
}

// Test next player functionality
TEST_F(GameModelTest, NextPlayer) {
    GameModel model;
    Player player1("Alice");
    Player player2("Bob");
    Player player3("Charlie");

    model.addPlayer(player1);
    model.addPlayer(player2);
    model.addPlayer(player3);

    // Initial current player should be Alice
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Alice");

    // Move to next player
    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Bob");

    // Move to next player
    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Charlie");

    // Move to next player (should wrap around)
    model.nextPlayer();
    EXPECT_EQ(model.getCurrentPlayer().getName(), "Alice");
}

// Test isOnlyOnePlayerAlive with multiple players
TEST_F(GameModelTest, IsOnlyOnePlayerAlive_MultiplePlayers) {
    GameModel model;
    Player player1("Alice");
    Player player2("Bob");

    model.addPlayer(player1);
    model.addPlayer(player2);

    EXPECT_FALSE(model.isOnlyOnePlayerAlive());
}

// Test isOnlyOnePlayerAlive with one player
TEST_F(GameModelTest, IsOnlyOnePlayerAlive_OnePlayer) {
    GameModel model;
    Player player1("Alice");

    model.addPlayer(player1);

    EXPECT_TRUE(model.isOnlyOnePlayerAlive());
}
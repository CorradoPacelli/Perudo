#include <gtest/gtest.h>

#include "Player.hpp"
#include "Bid.hpp"

class PlayerTest : public ::testing::Test {
protected:
    Player saitama{"Saitama", 5};
};

TEST_F(PlayerTest, InitialState) {
    EXPECT_EQ(saitama.getName(), "Saitama");
    EXPECT_EQ(saitama.getDiceCount(), 5);
    EXPECT_TRUE(saitama.isAlive());
    EXPECT_FALSE(saitama.isAI());
}

TEST_F(PlayerTest, LoseDie) {
    ASSERT_EQ(saitama.getDiceCount(), 5);
    saitama.loseDie();
    EXPECT_EQ(saitama.getDiceCount(), 4);
    EXPECT_TRUE(saitama.isAlive());
}

TEST_F(PlayerTest, BidHistory) {
    EXPECT_FALSE(saitama.getLastBid().has_value());

    Bid firstBid{3, 4};
    saitama.addBid(firstBid);
    ASSERT_TRUE(saitama.getLastBid().has_value());
    EXPECT_EQ(saitama.getLastBid().value(), firstBid);

    Bid secondBid{5, 5};
    saitama.addBid(secondBid);
    ASSERT_TRUE(saitama.getLastBid().has_value());
    EXPECT_EQ(saitama.getLastBid().value(), secondBid);

    const auto& history = saitama.getFullHistory();
    ASSERT_EQ(history.size(), 2);
    EXPECT_EQ(history[0], firstBid);
    EXPECT_EQ(history[1], secondBid);
}

TEST_F(PlayerTest, EqualityOperators) {
    Player genos{"Genos", 5};
    Player genos2{"Genos", 5};
    Player king{"King", 5};

    EXPECT_EQ(genos, genos);
    EXPECT_TRUE(genos == genos);

    EXPECT_NE(genos, king);
    EXPECT_TRUE(genos != king);

    EXPECT_NE(genos, genos2);
}

// Struct to hold parameters for the parameterized test for better readability
struct LoseDiceParams {
    int diceToLose;
    int expectedDiceCount;
    bool expectedIsAlive;
};

// Test fixture for the parameterized test, inheriting from PlayerTest
class PlayerLoseDiceTest : public PlayerTest,
                           public ::testing::WithParamInterface<LoseDiceParams> {};

INSTANTIATE_TEST_SUITE_P(
    LoseDiceScenarios,
    PlayerLoseDiceTest,
    ::testing::Values(
        LoseDiceParams{0, 5, true},   // Lose zero dice, should not change
        LoseDiceParams{3, 2, true},   // Lose some dice
        LoseDiceParams{5, 0, false},  // Lose exactly all dice
        LoseDiceParams{99, 0, false}  // Lose more than all dice, should be same as losing all
    ));

TEST_P(PlayerLoseDiceTest, CorrectlyLosesDiceAndUpdatesStatus) {
    const auto params = GetParam();
    saitama.loseDice(params.diceToLose);
    EXPECT_EQ(saitama.getDiceCount(), params.expectedDiceCount);
    EXPECT_EQ(saitama.isAlive(), params.expectedIsAlive);
}
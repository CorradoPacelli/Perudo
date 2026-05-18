#include <gtest/gtest.h>
#include <stdexcept>

#include "Bid.hpp"

class BidTest : public ::testing::Test {
};


TEST_F(BidTest, Accessors) {
    Bid bid(4, 5);
    EXPECT_EQ(bid.getQuantity(), 4);
    EXPECT_EQ(bid.getFace(), 5);
}

TEST_F(BidTest, ValidInitialization) {
    EXPECT_NO_THROW(Bid(2, 3));
    EXPECT_NO_THROW(Bid(1, 1));
    EXPECT_NO_THROW(Bid(10, 6));
}

TEST_F(BidTest, InvalidInitializationThrows) {
    // Invalid face (less than 1 or greater than 6)
    EXPECT_THROW(Bid(2, 0), std::invalid_argument);
    EXPECT_THROW(Bid(2, 7), std::invalid_argument);
    
    // Invalid quantity (less than 1)
    EXPECT_THROW(Bid(0, 3), std::invalid_argument);
    EXPECT_THROW(Bid(-5, 3), std::invalid_argument);
}

TEST_F(BidTest, GreaterThanOperatorLogic) {
    Bid previousBid(3, 4);

    // Valid: Higher quantity, any face
    EXPECT_TRUE(Bid(4, 2) > previousBid);
    EXPECT_TRUE(Bid(5, 6) > previousBid);

    // Valid: Same quantity, higher face
    EXPECT_TRUE(Bid(3, 5) > previousBid);
    EXPECT_TRUE(Bid(3, 6) > previousBid);

    // Invalid: Same quantity, lower or equal face
    EXPECT_FALSE(Bid(3, 4) > previousBid);
    EXPECT_FALSE(Bid(3, 2) > previousBid);

    // Invalid: Lower quantity, any face
    EXPECT_FALSE(Bid(2, 6) > previousBid);
    EXPECT_FALSE(Bid(1, 4) > previousBid);
}

TEST_F(BidTest, EqualityOperator) {
    Bid bid1(2, 5);
    Bid bid2(2, 5);
    Bid bid3(3, 5);
    Bid bid4(2, 4);

    EXPECT_TRUE(bid1 == bid2);
    EXPECT_FALSE(bid1 == bid3);
    EXPECT_FALSE(bid1 == bid4);
}
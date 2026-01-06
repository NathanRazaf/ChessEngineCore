#include <gtest/gtest.h>
#include "commons.h"

TEST(SquareUtilTest_GetSquareNotation, TestCorners) {
    EXPECT_EQ(getSquareNotation(0),  "a1");
    EXPECT_EQ(getSquareNotation(7),  "h1");
    EXPECT_EQ(getSquareNotation(56), "a8");
    EXPECT_EQ(getSquareNotation(63), "h8");
}

TEST(SquareUtilTest_GetSquareNotation, TestBorderSquares) {
    EXPECT_EQ(getSquareNotation(24), "a4");
    EXPECT_EQ(getSquareNotation(32), "a5");
    EXPECT_EQ(getSquareNotation(4),  "e1");
    EXPECT_EQ(getSquareNotation(60), "e8");
    EXPECT_EQ(getSquareNotation(31), "h4");
    EXPECT_EQ(getSquareNotation(39), "h5");
}

TEST(SquareUtilTest_GetSquareNotation, TestInnerSquares) {
    EXPECT_EQ(getSquareNotation(27), "d4");
    EXPECT_EQ(getSquareNotation(28), "e4");
    EXPECT_EQ(getSquareNotation(35), "d5");
    EXPECT_EQ(getSquareNotation(36), "e5");
}

TEST(SquareUtilTest_GetSquareNotation, ThrowsOnNegativeIndex) {
    EXPECT_THROW(getSquareNotation(-1), std::out_of_range);
}

TEST(SquareUtilTest_GetSquareNotation, ThrowsOnIndexTooLarge) {
    EXPECT_THROW(getSquareNotation(64), std::out_of_range);
    EXPECT_THROW(getSquareNotation(65), std::out_of_range);
    EXPECT_THROW(getSquareNotation(100), std::out_of_range);
}

TEST(SquareUtilTest_GetSquareIndex, TestCorners) {
    EXPECT_EQ(getSquareIndex("a8"), 56);
    EXPECT_EQ(getSquareIndex("h8"), 63);
    EXPECT_EQ(getSquareIndex("a1"),  0);
    EXPECT_EQ(getSquareIndex("h1"),  7);
}

TEST(SquareUtilTest_GetSquareIndex, TestBorderSquares) {
    EXPECT_EQ(getSquareIndex("a5"), 32);
    EXPECT_EQ(getSquareIndex("h5"), 39);
    EXPECT_EQ(getSquareIndex("e8"), 60);
    EXPECT_EQ(getSquareIndex("e1"),  4);
    EXPECT_EQ(getSquareIndex("a4"), 24);
    EXPECT_EQ(getSquareIndex("h4"), 31);
}                                   
                                    
TEST(SquareUtilTest_GetSquareIndex, TestInnerSquares) {
    EXPECT_EQ(getSquareIndex("e5"), 36);
    EXPECT_EQ(getSquareIndex("d4"), 27);
    EXPECT_EQ(getSquareIndex("d5"), 35);
    EXPECT_EQ(getSquareIndex("e4"), 28);
}

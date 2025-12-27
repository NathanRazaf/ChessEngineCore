#include <gtest/gtest.h>
#include "IPosition.h"

TEST(PositionTest_GetSquareNotation, TestCorners) {
    EXPECT_EQ(IPosition::get_square_notation(0), "a8");
    EXPECT_EQ(IPosition::get_square_notation(7), "h8");
    EXPECT_EQ(IPosition::get_square_notation(56), "a1");
    EXPECT_EQ(IPosition::get_square_notation(63), "h1");
}

TEST(PositionTest_GetSquareNotation, TestBorderSquares) {
    EXPECT_EQ(IPosition::get_square_notation(24), "a5");
    EXPECT_EQ(IPosition::get_square_notation(31), "h5");
    EXPECT_EQ(IPosition::get_square_notation(4), "e8");
    EXPECT_EQ(IPosition::get_square_notation(60), "e1");
    EXPECT_EQ(IPosition::get_square_notation(32), "a4");
    EXPECT_EQ(IPosition::get_square_notation(39), "h4");
}

TEST(PositionTest_GetSquareNotation, TestInnerSquares) {
    EXPECT_EQ(IPosition::get_square_notation(28), "e5");
    EXPECT_EQ(IPosition::get_square_notation(35), "d4");
    EXPECT_EQ(IPosition::get_square_notation(27), "d5");
    EXPECT_EQ(IPosition::get_square_notation(36), "e4");
}

TEST(PositionTest_GetSquareNotation, ThrowsOnNegativeIndex) {
    EXPECT_THROW(IPosition::get_square_notation(-1), std::out_of_range);
}

TEST(PositionTest_GetSquareNotation, ThrowsOnIndexTooLarge) {
    EXPECT_THROW(IPosition::get_square_notation(64), std::out_of_range);
    EXPECT_THROW(IPosition::get_square_notation(65), std::out_of_range);
    EXPECT_THROW(IPosition::get_square_notation(100), std::out_of_range);
}
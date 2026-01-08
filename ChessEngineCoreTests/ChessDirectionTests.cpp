#include <gtest/gtest.h>
#include "ChessDirections.h"
#include "commons.h"

TEST(ChessDirectionTest_TestHitsBorder, TestBorderSquares) {
	EXPECT_TRUE(hitsBorder(getSquareIndex("a6")));
	EXPECT_TRUE(hitsBorder(getSquareIndex("d8")));
	EXPECT_TRUE(hitsBorder(getSquareIndex("c1")));
	EXPECT_TRUE(hitsBorder(getSquareIndex("h5")));
}

TEST(ChessDirectionTest_TestHitsBorder, TestNonBorderSquares) {
	EXPECT_FALSE(hitsBorder(getSquareIndex("d6")));
	EXPECT_FALSE(hitsBorder(getSquareIndex("e3")));
	EXPECT_FALSE(hitsBorder(getSquareIndex("c2")));
	EXPECT_FALSE(hitsBorder(getSquareIndex("g7")));
}
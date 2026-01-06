#include <gtest/gtest.h>
#include "BBPosition.h"
#include "Piece.h"

TEST(BBPositionTest_FENTest, TestBaseBBPosition) {
    BBPosition p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Black BBPosition
    ASSERT_EQ(p.getPiece(0), createPiece(WHITE, ROOK));
    ASSERT_EQ(p.getPiece(1), createPiece(WHITE, KNIGHT));
    ASSERT_EQ(p.getPiece(2), createPiece(WHITE, BISHOP));
    ASSERT_EQ(p.getPiece(3), createPiece(WHITE, QUEEN));
    ASSERT_EQ(p.getPiece(4), createPiece(WHITE, KING));
    ASSERT_EQ(p.getPiece(5), createPiece(WHITE, BISHOP));
    ASSERT_EQ(p.getPiece(6), createPiece(WHITE, KNIGHT));
    ASSERT_EQ(p.getPiece(7), createPiece(WHITE, ROOK));
    ASSERT_EQ(p.getPiece(8), createPiece(WHITE, PAWN));
    // ...
    ASSERT_EQ(p.getPiece(15), createPiece(WHITE, PAWN));

    // White BBPosition
    ASSERT_EQ(p.getPiece(48), createPiece(BLACK, PAWN));
    // ...
    ASSERT_EQ(p.getPiece(55), createPiece(BLACK, PAWN));
    ASSERT_EQ(p.getPiece(56), createPiece(BLACK, ROOK));
    ASSERT_EQ(p.getPiece(57), createPiece(BLACK, KNIGHT));
    ASSERT_EQ(p.getPiece(58), createPiece(BLACK, BISHOP));
    ASSERT_EQ(p.getPiece(59), createPiece(BLACK, QUEEN));
    ASSERT_EQ(p.getPiece(60), createPiece(BLACK, KING));
    ASSERT_EQ(p.getPiece(61), createPiece(BLACK, BISHOP));
    ASSERT_EQ(p.getPiece(62), createPiece(BLACK, KNIGHT));
    ASSERT_EQ(p.getPiece(63), createPiece(BLACK, ROOK));

    ASSERT_EQ(p.getTurn(), WHITE);
}

TEST(BBPositionTest_FENTest, TestSimpleBBPosition) {
    BBPosition p("8/8/8/3k4/8/4K3/8/8 b - - 0 1");
    ASSERT_EQ(p.getPiece(20), createPiece(WHITE, KING));
    ASSERT_EQ(p.getPiece(35), createPiece(BLACK, KING));
    ASSERT_EQ(p.getTurn(), BLACK);
}

TEST(BBPositionTest_FENTest, TestCastlingRightsSetup) {
    BBPosition p("r3k2r/8/8/8/8/8/8/R3K2R w Kk - 0 1");
    ASSERT_EQ(p.getPiece(4), createPiece(WHITE, KING));
    ASSERT_EQ(p.getPiece(60), createPiece(BLACK, KING));
}

TEST(BBPositionTest_FENTest, TestWhiteEnPassant) {
    BBPosition p("4k3/1pp1pppp/8/p2pP3/8/8/PPPP1PPP/4K3 w - d6 0 1");

    ASSERT_EQ(p.getTurn(), WHITE);
}

TEST(BBPositionTest_FENTest, TestBlackEnPassant) {
    BBPosition p("4k3/ppp1pppp/8/8/3pP3/P7/1PPP1PPP/4K3 b - e3 0 1");

    ASSERT_EQ(p.getTurn(), BLACK);
}
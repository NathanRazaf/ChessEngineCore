#include <gtest/gtest.h>
#include "Position.h"
#include "Piece.h"

TEST(PositionTest_FENTest, TestBasePosition) {
    Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Black position
    ASSERT_EQ(p.get_piece(0), Piece::create_piece(BLACK, ROOK));
    ASSERT_EQ(p.get_piece(1), Piece::create_piece(BLACK, KNIGHT));
    ASSERT_EQ(p.get_piece(2), Piece::create_piece(BLACK, BISHOP));
    ASSERT_EQ(p.get_piece(3), Piece::create_piece(BLACK, QUEEN));
    ASSERT_EQ(p.get_piece(4), Piece::create_piece(BLACK, KING));
    ASSERT_EQ(p.get_piece(5), Piece::create_piece(BLACK, BISHOP));
    ASSERT_EQ(p.get_piece(6), Piece::create_piece(BLACK, KNIGHT));
    ASSERT_EQ(p.get_piece(7), Piece::create_piece(BLACK, ROOK));
    ASSERT_EQ(p.get_piece(8), Piece::create_piece(BLACK, PAWN));
    // ...
    ASSERT_EQ(p.get_piece(15), Piece::create_piece(BLACK, PAWN));

    // White position
    ASSERT_EQ(p.get_piece(48), Piece::create_piece(WHITE, PAWN));
    // ...
    ASSERT_EQ(p.get_piece(55), Piece::create_piece(WHITE, PAWN));
    ASSERT_EQ(p.get_piece(56), Piece::create_piece(WHITE, ROOK));
    ASSERT_EQ(p.get_piece(57), Piece::create_piece(WHITE, KNIGHT));
    ASSERT_EQ(p.get_piece(58), Piece::create_piece(WHITE, BISHOP));
    ASSERT_EQ(p.get_piece(59), Piece::create_piece(WHITE, QUEEN));
    ASSERT_EQ(p.get_piece(60), Piece::create_piece(WHITE, KING));
    ASSERT_EQ(p.get_piece(61), Piece::create_piece(WHITE, BISHOP));
    ASSERT_EQ(p.get_piece(62), Piece::create_piece(WHITE, KNIGHT));
    ASSERT_EQ(p.get_piece(63), Piece::create_piece(WHITE, ROOK));

    ASSERT_EQ(p.get_turn(), WHITE);
}

TEST(PositionTest_FENTest, TestSimplePosition) {
    Position p("8/8/8/3k4/8/4K3/8/8 b - - 0 1");
    ASSERT_EQ(p.get_piece(27), Piece::create_piece(BLACK, KING));
    ASSERT_EQ(p.get_piece(44), Piece::create_piece(WHITE, KING));
    ASSERT_EQ(p.get_turn(), BLACK);
}

TEST(PositionTest_FENTest, TestCastlingRightsSetup) {
    Position p("r3k2r/8/8/8/8/8/8/R3K2R w Kk - 0 1");
    ASSERT_EQ(p.get_piece(4), Piece::create_piece(BLACK, KING));
    ASSERT_EQ(p.get_piece(60), Piece::create_piece(WHITE, KING));
    
    // If castling right lost, rook should have moved at some point
    ASSERT_TRUE(Piece::has_moved(p.get_piece(0))); // Black queenside rook
    ASSERT_TRUE(Piece::has_moved(p.get_piece(56))); // White queenside rook

    // Else, rook shouldn't have moved because the castling right is preserved
    ASSERT_FALSE(Piece::has_moved(p.get_piece(7))); // Black kingside rook
    ASSERT_FALSE(Piece::has_moved(p.get_piece(63))); // White kingside rook
}

TEST(PositionTest_FENTest, TestWhiteEnPassant) {
    Position p("4k3/1pp1pppp/8/p2pP3/8/8/PPPP1PPP/4K3 w - d6 0 1");

    ASSERT_EQ(p.get_turn(), WHITE);
    // Black pawn on d5 can be captured en passant by white pawn on e5
    ASSERT_TRUE(Piece::can_be_captured_en_passant(p.get_piece(27)));
}

TEST(PositionTest_FENTest, TestBlackEnPassant) {
    Position p("4k3/ppp1pppp/8/8/3pP3/P7/1PPP1PPP/4K3 b - e3 0 1");

    ASSERT_EQ(p.get_turn(), BLACK);
    // White pawn on e4 can be captured en passant by black pawn on d4
    ASSERT_TRUE(Piece::can_be_captured_en_passant(p.get_piece(36)));
}
#include <gtest/gtest.h>
#include "Position.h"
#include "Piece.h"

TEST(PositionTest_FENTest, TestSimplePosition) {
    Position p("8/8/8/3k4/8/4K3/8/8 b - - 0 1");
    ASSERT_EQ(p.get_piece(27), Piece::create_piece(Color::BLACK, Type::KING));
    ASSERT_EQ(p.get_piece(44), Piece::create_piece(Color::WHITE, Type::KING));
    ASSERT_EQ(p.get_turn(), Color::BLACK);
}

TEST(PositionTest_FENTest, TestBasePosition) {
    Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Black position
    ASSERT_EQ(p.get_piece(0), Piece::create_piece(Color::BLACK, Type::ROOK));
    ASSERT_EQ(p.get_piece(1), Piece::create_piece(Color::BLACK, Type::KNIGHT));
    ASSERT_EQ(p.get_piece(2), Piece::create_piece(Color::BLACK, Type::BISHOP));
    ASSERT_EQ(p.get_piece(3), Piece::create_piece(Color::BLACK, Type::QUEEN));
    ASSERT_EQ(p.get_piece(4), Piece::create_piece(Color::BLACK, Type::KING));
    ASSERT_EQ(p.get_piece(5), Piece::create_piece(Color::BLACK, Type::BISHOP));
    ASSERT_EQ(p.get_piece(6), Piece::create_piece(Color::BLACK, Type::KNIGHT));
    ASSERT_EQ(p.get_piece(7), Piece::create_piece(Color::BLACK, Type::ROOK));
    ASSERT_EQ(p.get_piece(8), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(9), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(10), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(11), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(12), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(13), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(14), Piece::create_piece(Color::BLACK, Type::PAWN));
    ASSERT_EQ(p.get_piece(15), Piece::create_piece(Color::BLACK, Type::PAWN));

    // White position
    ASSERT_EQ(p.get_piece(48), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(49), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(50), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(51), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(52), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(53), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(54), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(55), Piece::create_piece(Color::WHITE, Type::PAWN));
    ASSERT_EQ(p.get_piece(56), Piece::create_piece(Color::WHITE, Type::ROOK));
    ASSERT_EQ(p.get_piece(57), Piece::create_piece(Color::WHITE, Type::KNIGHT));
    ASSERT_EQ(p.get_piece(58), Piece::create_piece(Color::WHITE, Type::BISHOP));
    ASSERT_EQ(p.get_piece(59), Piece::create_piece(Color::WHITE, Type::QUEEN));
    ASSERT_EQ(p.get_piece(60), Piece::create_piece(Color::WHITE, Type::KING));
    ASSERT_EQ(p.get_piece(61), Piece::create_piece(Color::WHITE, Type::BISHOP));
    ASSERT_EQ(p.get_piece(62), Piece::create_piece(Color::WHITE, Type::KNIGHT));
    ASSERT_EQ(p.get_piece(63), Piece::create_piece(Color::WHITE, Type::ROOK));

    ASSERT_EQ(p.get_turn(), Color::WHITE);
}
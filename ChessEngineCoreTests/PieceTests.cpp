#include <gtest/gtest.h>
#include "Piece.h"

TEST(PieceTest, CreateWhitePawn) {
    uint8_t piece = Piece::create_piece(WHITE, PAWN);
    EXPECT_TRUE(Piece::is_white(piece));
    EXPECT_EQ(Piece::get_type(piece), PAWN);
    EXPECT_FALSE(Piece::has_moved(piece));
    EXPECT_FALSE(Piece::can_be_captured_en_passant(piece));
}

TEST(PieceTest, CreateBlackKnight) {
    uint8_t piece = Piece::create_piece(BLACK, KNIGHT);
    EXPECT_FALSE(Piece::is_white(piece));
    EXPECT_EQ(Piece::get_type(piece), KNIGHT);
}

TEST(PieceTest, CreateWhiteKing) {
    uint8_t piece = Piece::create_piece(WHITE, KING);
    EXPECT_TRUE(Piece::is_white(piece));
    EXPECT_EQ(Piece::get_type(piece), KING);
}
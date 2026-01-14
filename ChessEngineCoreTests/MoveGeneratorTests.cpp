#include <gtest/gtest.h>
#include "MoveGenerator.h"
#include "BBPosition.h"
#include "commons.h"
#include <cstdint>
#include <bitset>

bool containsMove(const std::vector<Move>& moves, Square from, Square to, uint16_t flags = 0) {
    return std::find_if(moves.begin(), moves.end(), 
        [from, to, flags](const Move& m) {
            return m.from() == from && m.to() == to && 
                   (flags == 0 || (m.move & flags) == flags);
        }) != moves.end();
}

void printMoves(const std::vector<Move>& moves) {
    std::cout << "Total moves: " << moves.size() << std::endl;
    for (const auto& move : moves) {
        std::cout << "Move: " << move.from() << " -> " << move.to()
            << (move.isCapture() ? " CAPTURE" : "")
            << (move.isEnPassant() ? " EN_PASSANT" : "")
            << (move.isCastling() ? " IS_CASTLING" : "") 
            << (move.isPromotion() ? " IS_PROMOTION" : "")
            << std::endl;
    }
}

TEST(MoveGeneratorTest_TestPieces, TestMovesInSmallPosition) {
    BBPosition p("5B2/3R4/3Pkrp1/2P2p2/2K2b2/6p1/3p1q2/3n4 b - - 0 1");
    std::vector<Move> moves = p.getMoves();
    
    // printMoves(moves);

    // Black's turn
    // Rook moves
    EXPECT_TRUE(containsMove(moves, 45, 53));
    EXPECT_TRUE(containsMove(moves, 45, 61, IS_CAPTURE));
    EXPECT_FALSE(containsMove(moves, 45, 46)); // Allied piece
    // Bishop moves
    EXPECT_TRUE(containsMove(moves, 29, 20));
    EXPECT_TRUE(containsMove(moves, 29, 38));
    EXPECT_TRUE(containsMove(moves, 29, 47));
    EXPECT_TRUE(containsMove(moves, 29, 36));
    EXPECT_TRUE(containsMove(moves, 29, 43, IS_CAPTURE));
    EXPECT_FALSE(containsMove(moves, 29, 22)); // Allied piece
    EXPECT_FALSE(containsMove(moves, 29, 56)); // Wrapping file
    // Queen moves
    EXPECT_TRUE(containsMove(moves, 13, 12));
    EXPECT_TRUE(containsMove(moves, 13, 14));
    EXPECT_TRUE(containsMove(moves, 13, 20));
    EXPECT_TRUE(containsMove(moves, 13, 21));
    EXPECT_TRUE(containsMove(moves, 13, 6));
    EXPECT_TRUE(containsMove(moves, 13, 4));
    EXPECT_TRUE(containsMove(moves, 13, 34, IS_CAPTURE));
    // Knight moves
    EXPECT_TRUE(containsMove(moves, 3, 9));
    EXPECT_TRUE(containsMove(moves, 3, 18));
    EXPECT_TRUE(containsMove(moves, 3, 20));
    EXPECT_FALSE(containsMove(moves, 3, 13)); // Allied piece
    // King moves
    EXPECT_TRUE(containsMove(moves, 44, 43, IS_CAPTURE));
    EXPECT_TRUE(containsMove(moves, 44, 51, IS_CAPTURE));
    EXPECT_TRUE(containsMove(moves, 44, 36));
    EXPECT_FALSE(containsMove(moves, 44, 45)); // Allied piece
}

TEST(MoveGeneratorTest_TestPieces, TestWhitePawnMoves) {
    BBPosition p("4k3/8/8/1pPppp2/P4P2/8/3P4/4K3 w - d6 0 1");
    std::vector<Move> moves = p.getMoves();

    // printMoves(moves);

    // White's turn
    EXPECT_TRUE(containsMove(moves, 11, 19));
    EXPECT_TRUE(containsMove(moves, 11, 27)); // Double jump
    EXPECT_FALSE(containsMove(moves, 29, 37)); // Forward move blocked
    // Captures
    EXPECT_TRUE(containsMove(moves, 34, 43, IS_EN_PASSANT));
    EXPECT_TRUE(containsMove(moves, 24, 33, IS_CAPTURE));
}

TEST(MoveGeneratorTest_TestPieces, TestBlackPawnMoves) {
    BBPosition p("4k3/3p4/4p3/4p1p1/1Pp2P2/2PPP3/8/4K3 b - b3 0 1");
    std::vector<Move> moves = p.getMoves();

    // printMoves(moves);

    // Black's turn
    EXPECT_TRUE(containsMove(moves, 51, 43));
    EXPECT_TRUE(containsMove(moves, 51, 35)); // Double jump
    EXPECT_FALSE(containsMove(moves, 44, 36)); // Forward move blocked
    // Captures
    EXPECT_TRUE(containsMove(moves, 26, 17, IS_EN_PASSANT));
    EXPECT_TRUE(containsMove(moves, 26, 19, IS_CAPTURE));
}
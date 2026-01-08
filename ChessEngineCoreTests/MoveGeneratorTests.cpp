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

TEST(MoveGeneratorTest_TestSlidingPieces, TestMovesInSmallPosition) {
    BBPosition p("5B2/3R4/3Pkrp1/2P2p2/2K2b2/6p1/3p4/8 b - - 0 1");
    std::vector<Move> moves = p.getMoves();
    
    // printMoves(moves);

    // Black's turn
    // Rook moves
    EXPECT_TRUE(containsMove(moves, 45, 53));
    EXPECT_TRUE(containsMove(moves, 45, 61, IS_CAPTURE));
    // Bishop moves
    EXPECT_TRUE(containsMove(moves, 29, 20));
    EXPECT_TRUE(containsMove(moves, 29, 38));
    EXPECT_TRUE(containsMove(moves, 29, 47));
    EXPECT_TRUE(containsMove(moves, 29, 36));
    EXPECT_TRUE(containsMove(moves, 29, 43, IS_CAPTURE));
}
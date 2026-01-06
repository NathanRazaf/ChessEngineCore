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

TEST(MoveGeneratorTest_TestSlidingPieces, TestRookMovesInEmptyPosition) {
    BBPosition p("5B2/8/3Pkrp1/2PR1n2/3K4/8/8/8 b - - 0 1");
    std::vector<Move> moves = p.getMoves();
    
    // printMoves(moves);

    EXPECT_TRUE(containsMove(moves, 45, 53));
    EXPECT_TRUE(containsMove(moves, 45, 61, IS_CAPTURE));
}
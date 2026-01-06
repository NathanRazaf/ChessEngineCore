#pragma once
#include <cstdint>
#include <bit>
#include "types.h"
#include <string>
#include <stdexcept>

// Loop utilities
#define FOR_EACH_SQUARE(sq) for (Square sq = 0; sq < 64; sq++)
#define FOR_EACH_RANK(r) for (int r = 0; r < 8; r++)
#define FOR_EACH_FILE(f) for (int f = 0; f < 8; f++)
#define FOR_EACH_PIECE(p) for (Piece p = 0; p < 16; p++)

// Square utilities
constexpr Square boardSquare(int rank, int file) {
    return rank * 8 + file;
}

constexpr int boardRank(Square square) {
    return square / 8;
}

constexpr int boardFile(Square square) {
    return square % 8;
}

constexpr bool isSquareValid(Square square) {
    return square >= 0 && square < 64;
}

inline std::string getSquareNotation(Square square) {
    if (!isSquareValid(square)) {
        throw std::out_of_range("Square index out of bounds");
    }
    int file = boardFile(square);
    int rank = boardRank(square);
    return std::string{ char('a' + file), char('1' + rank) };
}

inline Square getSquareIndex(const std::string& notation) {
    char file = notation[0];
    char rank = notation[1];
    int rankIndex = rank - '1';  // '1' -> 0, '8' -> 7
    int fileIndex = file - 'a';  // 'a' -> 0, 'h' -> 7
    return rankIndex * 8 + fileIndex;
}

// Bitboard operations
inline void setBit(Bitboard& bb, int sq) {
    bb |= (1ULL << sq);
}

inline void unsetBit(Bitboard& bb, int sq) {
    bb &= ~(1ULL << sq);
}

inline bool getBit(Bitboard bb, int sq) {
    return (bb >> sq) & 1ULL;
}

inline void popLsb(Bitboard& bb) {
    bb &= bb - 1;
}

inline int countBits(Bitboard bb) {
    return std::popcount(bb);
}

inline int getLsb(Bitboard bb) {
    return std::countr_zero(bb);
}

inline std::vector<Square> getSquareList(Bitboard bb) {
    Bitboard bbCopy = bb;
    std::vector<Square> squares;
    squares.reserve(64);

    while (bbCopy) {
        // Get position of lowest bit in bitboard
        Square square = getLsb(bbCopy);  // Count trailing zeros

        squares.push_back(square);

        // Remove this bit from bitboard and move to next
        popLsb(bbCopy);
    }

    return squares;
}
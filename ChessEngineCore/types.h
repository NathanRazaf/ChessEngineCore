#pragma once
#include <cstdint>

enum Color {
    WHITE = 0,
    BLACK = 8
};

enum Type {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
};


enum CastlingType {
    W_KING_SIDE = 0b0001,
    W_QUEEN_SIDE = 0b0010,
    B_KING_SIDE = 0b0100,
    B_QUEEN_SIDE = 0b1000
};

using CastlingRights = uint8_t;

using Bitboard = uint64_t;
using Square = uint8_t;
using Piece = uint8_t;

constexpr uint8_t EMPTY = 0;
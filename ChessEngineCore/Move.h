#pragma once
#include <cstdint>
#include "types.h"

enum MoveFlags {
    IS_CAPTURE    = 1 << 12,
    IS_EN_PASSANT = 1 << 13,
    IS_CASTLING   = 1 << 14,
    IS_PROMOTION  = 1 << 15
};

struct Move {
    uint16_t move;

public:
    Move() : move(0) {}

    Move(Square from, Square to) : move((from << 6) | to) {}

    Move(Square from, Square to, uint16_t flags)
        : move((from << 6) | to | flags) {
    }

    int from() const { return (move >> 6) & 0x3F; }
    int to() const { return move & 0x3F; }
    bool isCapture() const { return move & IS_CAPTURE; }
    bool isEnPassant() const { return move & IS_EN_PASSANT; }
    bool isCastling() const { return move & IS_CASTLING; }
    bool isPromotion() const { return move & IS_PROMOTION; }

    bool operator==(const Move& other) const {
        return move == other.move;
    }
};
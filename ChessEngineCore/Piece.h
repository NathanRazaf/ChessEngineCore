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

enum Flag {
    HAS_MOVED = 16,
    CAN_BE_EN_PASSANT_CAPTURED = 32
};

class Piece {
public:
    static uint8_t create_piece(Color col, Type t);
    static Type get_type(uint8_t piece);
    static bool is_white(uint8_t piece);
    static bool has_moved(uint8_t piece);
    static void set_moved(uint8_t& piece);
    static void unset_moved(uint8_t& piece);
    static bool can_be_captured_en_passant(uint8_t pawn);
    static void set_en_passant_valid(uint8_t& pawn);
    static void set_en_passant_invalid(uint8_t& pawn);
};

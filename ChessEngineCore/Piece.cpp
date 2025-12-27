#include "Piece.h"

uint8_t Piece::create_piece(Color col, Type t) {
    return t | col;
}

Type Piece::get_type(uint8_t piece) {
    return static_cast<Type>(piece & 7);
}

bool Piece::is_white(uint8_t piece) {
    return (piece & BLACK) == 0;
}

bool Piece::has_moved(uint8_t piece) {
    return (piece & HAS_MOVED) == HAS_MOVED;
}

void Piece::set_moved(uint8_t& piece) {
    piece |= HAS_MOVED; 
}

void Piece::unset_moved(uint8_t& piece) {
    piece &= ~HAS_MOVED;
}

bool Piece::can_be_captured_en_passant(uint8_t pawn) {
    return (pawn & CAN_BE_EN_PASSANT_CAPTURED) == CAN_BE_EN_PASSANT_CAPTURED;
}

void Piece::set_en_passant_valid(uint8_t& pawn) {
    pawn |= CAN_BE_EN_PASSANT_CAPTURED;
}

void Piece::set_en_passant_invalid(uint8_t& pawn) {
    pawn &= ~CAN_BE_EN_PASSANT_CAPTURED;
}


#include "Piece.h"
#include <cstdint>

class Piece {
	uint8_t create_piece(Color col, Type t) {
		return t | col;
	}

	Type get_type(uint8_t piece) {
		return static_cast<Type>(piece & 7);
	}

	bool is_white(uint8_t piece) {
		return (piece & Color::BLACK) == 0;
	}

	bool has_moved(uint8_t piece) {
		return (piece & Flag::HAS_MOVED) == 16;
	}

	bool can_be_captured_en_passant(uint8_t pawn) {
		return (pawn & Flag::CAN_BE_EN_PASSANT_CAPTURED) == 32;
	}
};
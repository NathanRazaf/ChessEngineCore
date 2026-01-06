#pragma once
#include "types.h"

inline Piece createPiece(Color color, Type type) {
	return static_cast<Piece>(color | type);
}

inline Type getPieceType(Piece piece) {
	return static_cast<Type>(piece & 0b00000111);
}

inline Color getPieceColor(Piece piece) {
	return static_cast<Color>(piece & 0b00001000);
}
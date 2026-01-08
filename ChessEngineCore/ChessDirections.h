#pragma once
#include "types.h"

enum ChessDirection {
	NORTH = 8,
	SOUTH = -8,
	WEST = -1,
	EAST = 1,
	NORTH_WEST = 7,
	NORTH_EAST = 9,
	SOUTH_WEST = -9,
	SOUTH_EAST = -7
};

constexpr Bitboard FileABB = 0x0101010101010101ULL;
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;

constexpr Bitboard Rank1BB = 0xFF;
constexpr Bitboard Rank2BB = Rank1BB << (8 * 1);
constexpr Bitboard Rank3BB = Rank1BB << (8 * 2);
constexpr Bitboard Rank4BB = Rank1BB << (8 * 3);
constexpr Bitboard Rank5BB = Rank1BB << (8 * 4);
constexpr Bitboard Rank6BB = Rank1BB << (8 * 5);
constexpr Bitboard Rank7BB = Rank1BB << (8 * 6);
constexpr Bitboard Rank8BB = Rank1BB << (8 * 7);

constexpr bool hitsBorder(Square square) {
	Bitboard squareBB = 1ULL << square;
	return (FileABB & squareBB) ||
		(FileHBB & squareBB) ||
		(Rank1BB & squareBB) ||
		(Rank8BB & squareBB);
}
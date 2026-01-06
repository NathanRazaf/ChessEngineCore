#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "IPosition.h"

class Position : public IPosition {
private:
	void clearBoard() override;
public:
	Position();
	Position(const std::string& fen);
	Position(const Position& other);

	uint8_t getPiece(Square sq) const override;
	void setPiece(Square sq, uint8_t piece) override;
	void setPosition(const std::string& fen) override;
	void doMove(Square start, Square end) override;
	std::vector<Move> getMoves() const override;
	Bitboard getOccupancyBitboard() const override;
};
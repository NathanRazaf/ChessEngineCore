#pragma once
#include "IPosition.h"

class BBPosition : public IPosition {
private:
	// Pieces
	Bitboard m_pieceBitboards[16];

	void clearBoard() override;
public:
	BBPosition();
	BBPosition(const std::string& fen);
	Piece getPiece(Square sq) const override;
	void setPiece(Square sq, Piece piece) override;
	void setPosition(const std::string& fen) override;
	void doMove(Square start, Square end) override;
	std::vector<Move> getMoves() const override;
	Bitboard getOccupancyBitboard() const override;
	Bitboard getColorBitboard(Color color) const;
	Bitboard getPieceBitboard(Piece piece) const {
		return m_pieceBitboards[piece];
	}
};
#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "IPosition.h"

class Position : public IPosition {
private:
	uint8_t board[64];

	std::vector<int> get_sliding_moves(uint8_t piece, int square) const;
	void clear_board();

public:
	Position();
	Position(const std::string& fen);
	Position(const Position& other);

	uint8_t get_piece(int square) const override;
	void set_piece(int square, uint8_t piece) override;
	void set_position(const std::string& fen) override;
	void move(int start, int end);
	std::vector<int> get_moves(int square) const override;
};
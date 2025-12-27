#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "Piece.h"

class IPosition {
protected:
    bool is_white_turn;

public:
    virtual ~IPosition() = default; 

    // (= 0 means "must be implemented")
    virtual uint8_t get_piece(int square) const = 0;
    virtual void set_piece(int square, uint8_t piece) = 0;
    virtual void set_position(const std::string& fen) = 0;
    virtual std::vector<int> get_moves(int square) const = 0;
    
    Color get_turn() const {
        return is_white_turn ? WHITE : BLACK;
    };

    bool is_empty(int square) const {
        return get_piece(square) == 0;
    }

    static bool is_square_valid(int square) {
        return square >= 0 && square < 64;
    }

    static std::string get_square_notation(int square) {
        if (!is_square_valid(square)) {
            throw std::out_of_range("Square index out of bounds");
        }

        int file = square % 8;
        int rank = 8 - (square / 8);

        return std::string{ char('a' + file), char('0' + rank) };
    }

    static int get_square_index(std::string notation) {
        char file = notation[0];
        char rank = notation[1];

        int mult = ('8' - rank) * 8;
        int modulo = file - 'a';
        
        return mult + modulo;
    }
};
#include "Position.h"
#include "commons.h"
#include "Piece.h"
#include <map>
#include <cctype>
#include <iostream>

Position::Position() {
    clearBoard();
    m_isWhiteTurn = true;
}

Position::Position(const std::string& fen) {
    clearBoard();
    setPosition(fen);
}

Position::Position(const Position& other) {
    std::copy(other.board, other.board + 64, board);
    m_isWhiteTurn = other.m_isWhiteTurn;
}

void Position::doMove(Square start, Square end) {
    if (!isSquareValid(start)) {
        throw std::out_of_range("Start square index out of bounds");
    }
    if (!isSquareValid(end)) {
        throw std::out_of_range("End square index out of bounds");
    }

    m_halfmoveClock++;
    // Reset halfmove clock if the piece moved is a pawn or if the end square had a piece
    if ((getPieceType(getPiece(start)) == PAWN) ||
        (getPiece(end) != 0)) {
        m_halfmoveClock = 0;
    };

    // Increment fullmoves if black's turn
    if (getTurn() == BLACK) {
        m_fullmoves++;
    }
    
    uint8_t piece = board[start];
    board[end] = piece;
    board[start] = 0;

    // Swap turns
    m_isWhiteTurn = !m_isWhiteTurn;
}

uint8_t Position::getPiece(Square square) const {
    if (!isSquareValid(square)) {
        throw std::out_of_range("Square index out of bounds");
    }
    return board[square];
}

void Position::setPiece(Square square, uint8_t piece) {
    if (!isSquareValid(square)) {
        throw std::out_of_range("Square index out of bounds");
    }
    board[square] = piece;
};


void Position::setPosition(const std::string& fen) {
}

std::vector<Move> Position::getMoves() const {
    return {};
}

void Position::clearBoard() {
    std::fill(board, board + 64, 0);
}

Bitboard Position::getOccupancyBitboard() const {
    Bitboard bitboard = 0;
    FOR_EACH_SQUARE(square) {
        if (getPiece(square) != 0) {
            setBit(bitboard, square);
        }
    }

    return bitboard;
}

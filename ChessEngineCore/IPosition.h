#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "types.h"
#include "commons.h"
#include "Move.h"

class IPosition {
protected:
    bool m_isWhiteTurn = true;
    int m_halfmoveClock = 0;
    int m_fullmoves = 0;

    CastlingRights cr;

    Piece board[64];

    // En passant target square
    Square m_enPassantSquare = EMPTY;

    virtual void clearBoard() = 0;
    inline bool hasWhiteKingCastling() { return cr & W_KING_SIDE; }
    inline bool hasWhiteQueenCastling() { return cr & W_QUEEN_SIDE; }
    inline bool hasBlackKingCastling() { return cr & B_KING_SIDE; }
    inline bool hasBlackQueenCastling() { return cr & B_QUEEN_SIDE; }
    inline void addCr(CastlingType ct) { cr |= ct; }
    inline void removeCr(CastlingType ct) { cr &= ~ct; }
public:
    virtual ~IPosition() = default;

    // (= 0 means "must be implemented")
    virtual Piece getPiece(Square sq) const = 0;
    virtual void setPiece(Square sq, Piece piece) = 0;
    virtual void setPosition(const std::string& fen) = 0;
    virtual void doMove(Square start, Square end) = 0;
    virtual std::vector<Move> getMoves() const = 0;
    virtual Bitboard getOccupancyBitboard() const = 0;

    Color getTurn() const {
        return m_isWhiteTurn ? WHITE : BLACK;
    }

    int getHalfmoveClock() const {
        return m_halfmoveClock;
    }

    int getFullmovesNumber() const {
        return m_fullmoves;
    }

    Square getEnPassantSquare() const {
        return m_enPassantSquare;
    }

    bool isEmpty(int square) const {
        return getPiece(square) == 0;
    }
};
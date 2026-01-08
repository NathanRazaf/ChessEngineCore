#include "MoveGenerator.h"
#include "types.h"
#include "Piece.h"
#include "Move.h"
#include "ChessDirections.h"
#include <bit>
#include <iostream>

// Define static member variables
MoveGenerator* MoveGenerator::s_instance = nullptr;
Bitboard MoveGenerator::RookAttackTable[102400] = {};
Bitboard MoveGenerator::BishopAttackTable[5248] = {};

void MoveGenerator::initializeTables() {
    initRookMasks();
    initBishopMasks();
    initSlidingOffsets();
    initRookAttackTable();
    initBishopAttackTable();
}

void MoveGenerator::initRookMasks() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = 0;
        int rank = boardRank(square);
        int file = boardFile(square);

        // North (increasing rank, stop before edge)
        for (int r = rank + 1; r < 7; r++) {
            setBit(mask, r * 8 + file);
        }

        // South (decreasing rank, stop before edge)
        for (int r = rank - 1; r > 0; r--) {
            setBit(mask, r * 8 + file);
        }

        // East (stop before edge)
        for (int f = file + 1; f < 7; f++) {
            setBit(mask, rank * 8 + f);
        }

        // West (stop before edge)
        for (int f = file - 1; f > 0; f--) {
            setBit(mask, rank * 8 + f);
        }

        m_RookMasks[square] = mask;
    }
}

void MoveGenerator::initBishopMasks() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = 0;
        int rank = boardRank(square);
        int file = boardFile(square);
        int f, r;

        // North-West (increasing rank, decreasing file, stop before edge)
        f = file - 1;
        r = rank + 1;
        while (f > 0 && r < 7) {
            setBit(mask, r * 8 + f);
            f--;
            r++;
        }

        // North-East (increasing rank, increasing file, stop before edge)
        f = file + 1;
        r = rank + 1;
        while (f < 7 && r < 7) {
            setBit(mask, r * 8 + f);
            f++;
            r++;
        }

        // South-East (decreasing rank, increasing file, stop before edge)
        f = file + 1;
        r = rank - 1;
        while (f < 7 && r > 0) {
            setBit(mask, r * 8 + f);
            f++;
            r--;
        }

        // South-West (decreasing rank, decreasing file, stop before edge)
        f = file - 1;
        r = rank - 1;
        while (f > 0 && r > 0) {
            setBit(mask, r * 8 + f);
            f--;
            r--;
        }

        m_BishopMasks[square] = mask;
    }
}

void MoveGenerator::initSlidingOffsets() {
    m_RookOffsets[0] = 0;
    m_BishopOffsets[0] = 0;

    for (int square = 1; square < 64; ++square) {
        // ROOKS
        // Count bits in previous square's mask
        int prevBits = countBits(m_RookMasks[square - 1]);
        int prevSize = 1 << prevBits;  // 2^bits = number of variations

        // Current square starts after previous square's entries
        m_RookOffsets[square] = m_RookOffsets[square - 1] + prevSize;


        // BISHOPS
        // Count bits in previous square's mask
        prevBits = countBits(m_BishopMasks[square - 1]);
        prevSize = 1 << prevBits; // 2^bits = number of variations

        // Current square starts after previous square's entries
        m_BishopOffsets[square] = m_BishopOffsets[square - 1] + prevSize;
    }
}

void MoveGenerator::initRookAttackTable() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = m_RookMasks[square];
        int numBits = countBits(mask);
        int numOccupancies = 1 << numBits;

        for (int i = 0; i < numOccupancies; i++) {
            Bitboard occupancy = indexToOccupancy(i, mask);
            Bitboard attacks = generateRookAttacksSlow(square, occupancy);

            // Magic hash
            uint64_t index = (occupancy * m_RookMagics[square]) >> m_RookShifts[square];

            // Store!
            RookAttackTable[m_RookOffsets[square] + index] = attacks;
        }
    }
}

void MoveGenerator::initBishopAttackTable() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = m_BishopMasks[square];
        int numBits = countBits(mask);
        int numOccupancies = 1 << numBits;

        for (int i = 0; i < numOccupancies; i++) {
            Bitboard occupancy = indexToOccupancy(i, mask);
            Bitboard attacks = generateBishopAttacksSlow(square, occupancy);

            // Magic hash
            uint64_t index = (occupancy * m_BishopMagics[square]) >> m_BishopShifts[square];

            // Store!
            BishopAttackTable[m_BishopOffsets[square] + index] = attacks;
        }
    }
}

Bitboard MoveGenerator::generateRookAttacksSlow(int square, Bitboard occupied) const {
    Bitboard attacks = 0;
    
    for (ChessDirection direction : {NORTH, SOUTH, EAST, WEST}) {
        Square sq = square;
        int startRank = boardRank(square);

        while (true) {
            sq += direction;

            // Went off board
            if (sq < 0 || sq >= 64) break;

            // For horizontal moves, check if we wrapped to different rank
            if ((direction == EAST || direction == WEST) &&
                boardRank(sq) != startRank) break;

            setBit(attacks, sq);
            if (occupied & (1ULL << sq)) break;
            if (hitsBorder(sq)) break;
        }
    }

    return attacks;
}

Bitboard MoveGenerator::generateBishopAttacksSlow(int square, Bitboard occupied) const {
    Bitboard attacks = 0;

    for (ChessDirection direction : {NORTH_EAST, SOUTH_EAST, NORTH_WEST, SOUTH_WEST}) {
        Square sq = square;
        int currentRank = boardRank(square);

        while (true) {
            sq += direction;

            // Went off board
            if (sq < 0 || sq >= 64) break;

            // Check if we wrapped to 2 ranks forwards
            if ((direction == NORTH_EAST || direction == NORTH_WEST) &&
                (boardRank(sq) == currentRank + 2 || boardRank(sq) == currentRank)) break;
            // Check if we wrapped to 2 ranks backwards
            if ((direction == SOUTH_EAST || direction == SOUTH_WEST) &&
                (boardRank(sq) == currentRank - 2 || boardRank(sq) == currentRank)) break;

            setBit(attacks, sq);
            if (occupied & (1ULL << sq)) break; // Captures an enemy piece
            if (hitsBorder(sq)) break; // Hits the chessboard border
            currentRank = boardRank(sq);
        }
    }

    return attacks;
}

void MoveGenerator::insertRookMoves(const BBPosition& position, std::vector<Move>& moves, Bitboard alliedPieces, Bitboard enemyPieces) const {
    Bitboard rookPositions = position.getPieceBitboard(createPiece(position.getTurn(), ROOK));
    Bitboard allPieces = alliedPieces | enemyPieces;

    while (rookPositions) {
        Square square = getLsb(rookPositions);
        popLsb(rookPositions);
        Bitboard occupied = allPieces & m_RookMasks[square];
        uint64_t index = (occupied * m_RookMagics[square]) >> m_RookShifts[square];

        Bitboard attacks = RookAttackTable[m_RookOffsets[square] + index];

        // Remove friendly pieces
        attacks &= ~alliedPieces;

        // Separate captures from quiet moves
        Bitboard captures = attacks & enemyPieces;
        Bitboard quietMoves = attacks & ~enemyPieces;

        while (captures) {
            Square target = getLsb(captures);
            popLsb(captures);
            moves.emplace_back(Move(square, target, IS_CAPTURE));
        }

        while (quietMoves) {
            Square target = getLsb(quietMoves);
            popLsb(quietMoves);
            moves.emplace_back(Move(square, target));
        }
    }
}

void MoveGenerator::insertBishopMoves(const BBPosition& position, std::vector<Move>& moves, Bitboard alliedPieces, Bitboard enemyPieces) const {
    Bitboard bishopPositions = position.getPieceBitboard(createPiece(position.getTurn(), BISHOP));
    Bitboard allPieces = alliedPieces | enemyPieces;

    while (bishopPositions) {
        Square square = getLsb(bishopPositions);
        popLsb(bishopPositions);
        Bitboard occupied = allPieces & m_BishopMasks[square];
        uint64_t index = (occupied * m_BishopMagics[square]) >> m_BishopShifts[square];

        Bitboard attacks = BishopAttackTable[m_BishopOffsets[square] + index];

        // Remove friendly pieces
        attacks &= ~alliedPieces;

        // Separate captures from quiet moves
        Bitboard captures = attacks & enemyPieces;
        Bitboard quietMoves = attacks & ~enemyPieces;

        while (captures) {
            Square target = getLsb(captures);
            popLsb(captures);
            moves.emplace_back(Move(square, target, IS_CAPTURE));
        }

        while (quietMoves) {
            Square target = getLsb(quietMoves);
            popLsb(quietMoves);
            moves.emplace_back(Move(square, target));
        }
    }
}

std::vector<Move> MoveGenerator::generateMoves(const BBPosition& position) const {
    std::vector<Move> moves = {};
    moves.reserve(218); // There are theoretically at most 218 possible moves in a position

    Color us = position.getTurn();
    Color against = us == BLACK ? WHITE : BLACK;
    Bitboard alliedPieces = position.getColorBitboard(us);
    Bitboard enemyPieces = position.getColorBitboard(against);
    insertRookMoves(position, moves, alliedPieces, enemyPieces);
    insertBishopMoves(position, moves, alliedPieces, enemyPieces);

    return moves;
}

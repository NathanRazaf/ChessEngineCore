#include "MoveGenerator.h"
#include "types.h"
#include "Piece.h"
#include "Move.h"
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

Bitboard MoveGenerator::indexToOccupancy(int index, Bitboard mask) const {
    Bitboard occupancy = 0;
    Bitboard maskCopy = mask;
    int bitCount = 0;

    while (maskCopy) {
        // Get position of lowest bit in mask
        int bitPos = std::countr_zero(maskCopy);  // Count trailing zeros

        // If this bit is set in the index, set it in occupancy
        if (index & (1 << bitCount)) {
            setBit(occupancy, bitPos);
        }

        // Remove this bit from mask and move to next
        popLsb(maskCopy);  // Clear lowest bit
        bitCount++;
    }

    return occupancy;
}

Bitboard MoveGenerator::generateRookAttacksSlow(int square, Bitboard occupied) const {
    Bitboard attacks = 0;
    int rank = boardRank(square);
    int file = boardFile(square);

    // North - slide until hitting a piece or edge
    for (int r = rank + 1; r < 8; r++) {
		Square sq = boardSquare(r, file);
        setBit(attacks, sq);
        if (occupied & (1ULL << sq)) break;  // Hit a blocker
    }

    // South
    for (int r = rank - 1; r >= 0; r--) {
        Square sq = boardSquare(r, file);
        setBit(attacks, sq);
        if (occupied & (1ULL << sq)) break;
    }

    // East
    for (int f = file + 1; f < 8; f++) {
        Square sq = boardSquare(rank, f);
        setBit(attacks, sq);
        if (occupied & (1ULL << sq)) break;
    }

    // West
    for (int f = file - 1; f >= 0; f--) {
        Square sq = boardSquare(rank, f);
        setBit(attacks, sq);
        if (occupied & (1ULL << sq)) break;
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

std::vector<Move> MoveGenerator::generateMoves(const BBPosition& position) const {
    std::vector<Move> moves = {};
    moves.reserve(218); // There are theoretically at most 218 possible moves in a position

    Color us = position.getTurn();
    Color against = us == BLACK ? WHITE : BLACK;
    Bitboard alliedPieces = position.getColorBitboard(us);
    Bitboard enemyPieces = position.getColorBitboard(against);
    insertRookMoves(position, moves, alliedPieces, enemyPieces);

    return moves;
}

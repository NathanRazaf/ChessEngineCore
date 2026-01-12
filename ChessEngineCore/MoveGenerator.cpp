#include "MoveGenerator.h"
#include "ChessDirections.h"
#include <bit>
#include <iostream>

// Define static member variables
MoveGenerator* MoveGenerator::s_instance = nullptr;
Bitboard MoveGenerator::m_RookAttackTable[102400]  = {};
Bitboard MoveGenerator::m_BishopAttackTable[5248]  = {};
Bitboard MoveGenerator::m_KnightAttackTable[64]    = {};
Bitboard MoveGenerator::m_KingAttackTable[64]      = {};
Bitboard MoveGenerator::m_WhitePawnAttackTable[64] = {};
Bitboard MoveGenerator::m_BlackPawnAttackTable[64] = {};

void MoveGenerator::initializeTables() {
    initRookMasks();
    initBishopMasks();
    initSlidingOffsets();
    initRookAttackTable();
    initBishopAttackTable();
    initKnightAttackTable();
}

void MoveGenerator::initRookMasks() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = 0;
        int startRank = boardRank(square);

        for (ChessDirection direction : {NORTH, SOUTH, EAST, WEST}) {
            Square sq = square;

            while (true) {
                sq += direction;

                // Went off board
                if (sq < 0 || sq >= 64) break;

                // For horizontal moves, check if we wrapped to different rank
                if ((direction == EAST || direction == WEST) &&
                    boardRank(sq) != startRank) break;

                // Stop before hitting the edge 
                if (hitsBorder(sq)) break;

                setBit(mask, sq);
            }
        }

        m_RookMasks[square] = mask;
    }
}

void MoveGenerator::initBishopMasks() {
    FOR_EACH_SQUARE(square) {
        Bitboard mask = 0;
        int currentRank = boardRank(square);

        for (ChessDirection direction : {NORTH_EAST, SOUTH_EAST, NORTH_WEST, SOUTH_WEST}) {
            Square sq = square;
            currentRank = boardRank(square);

            while (true) {
                sq += direction;

                // Went off board
                if (sq < 0 || sq >= 64) break;

                // Check if we wrapped to the same rank
                if (boardRank(sq) == currentRank) break;

                // Check if we wrapped to 2 ranks forwards
                if ((direction == NORTH_EAST || direction == NORTH_WEST) &&
                    boardRank(sq) == currentRank + 2) break;
                // Check if we wrapped to 2 ranks backwards
                if ((direction == SOUTH_EAST || direction == SOUTH_WEST) &&
                    boardRank(sq) == currentRank - 2) break;

                // Stop before hitting the edge 
                if (hitsBorder(sq)) break;

                setBit(mask, sq);
                currentRank = boardRank(sq);
            }
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
            m_RookAttackTable[m_RookOffsets[square] + index] = attacks;
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
            m_BishopAttackTable[m_BishopOffsets[square] + index] = attacks;
        }
    }
}

void MoveGenerator::initKnightAttackTable() {
    int octopus[] = { -17, -15, -10, -6, 6, 10, 15, 17 };
    FOR_EACH_SQUARE(square) {
        Bitboard attacks = 0;
        int rank = boardRank(square);
        int file = boardFile(square);

        for (int offset : octopus) {
            int target = square + offset;

            // Bounds check
            if (target < 0 || target >= 64) continue;

            int targetRank = boardRank(target);
            int targetFile = boardFile(target);

            // Check if move wraps around board edges
            int rankDiff = abs(targetRank - rank);
            int fileDiff = abs(targetFile - file);

            // Knight moves: 2 squares one direction, 1 square other direction
            if ((rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2)) {
                attacks |= (1ULL << target);
            }
        }

        m_KnightAttackTable[square] = attacks;
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

            // Check if we wrapped to the same rank
            if (boardRank(sq) == currentRank) break;

            // Check if we wrapped to 2 ranks forwards
            if ((direction == NORTH_EAST || direction == NORTH_WEST) &&
                boardRank(sq) == currentRank + 2) break;
            // Check if we wrapped to 2 ranks backwards
            if ((direction == SOUTH_EAST || direction == SOUTH_WEST) &&
                boardRank(sq) == currentRank - 2) break;

            setBit(attacks, sq);
            if (occupied & (1ULL << sq)) break; // Captures an enemy piece
            if (hitsBorder(sq)) break; // Hits the chessboard border
            currentRank = boardRank(sq);
        }
    }

    return attacks;
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
    insertQueenMoves(position, moves, alliedPieces, enemyPieces);
    insertKnightMoves(position, moves, alliedPieces, enemyPieces);

    return moves;
}

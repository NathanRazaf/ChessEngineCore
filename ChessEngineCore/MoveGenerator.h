#pragma once
#include <vector>
#include "Move.h"
#include "commons.h"
#include "BBPosition.h"

class MoveGenerator {
private:
    static MoveGenerator* s_instance;
    // Rook magics for each square (0-63)
    const Bitboard m_RookMagics[64] = {
        0x0080001020400080ULL, 0x0040001000200040ULL, 0x0080081000200080ULL, 0x0080040800100080ULL,
        0x0080020400080080ULL, 0x0080010200040080ULL, 0x0080008001000200ULL, 0x0080002040800100ULL,
        0x0000800020400080ULL, 0x0000400020005000ULL, 0x0000801000200080ULL, 0x0000800800100080ULL,
        0x0000800400080080ULL, 0x0000800200040080ULL, 0x0000800100020080ULL, 0x0000800040800100ULL,
        0x0000208000400080ULL, 0x0000404000201000ULL, 0x0000808010002000ULL, 0x0000808008001000ULL,
        0x0000808004000800ULL, 0x0000808002000400ULL, 0x0000010100020004ULL, 0x0000020000408104ULL,
        0x0000208080004000ULL, 0x0000200040005000ULL, 0x0000100080200080ULL, 0x0000080080100080ULL,
        0x0000040080080080ULL, 0x0000020080040080ULL, 0x0000010080800200ULL, 0x0000800080004100ULL,
        0x0000204000800080ULL, 0x0000200040401000ULL, 0x0000100080802000ULL, 0x0000080080801000ULL,
        0x0000040080800800ULL, 0x0000020080800400ULL, 0x0000020001010004ULL, 0x0000800040800100ULL,
        0x0000204000808000ULL, 0x0000200040008080ULL, 0x0000100020008080ULL, 0x0000080010008080ULL,
        0x0000040008008080ULL, 0x0000020004008080ULL, 0x0000010002008080ULL, 0x0000004081020004ULL,
        0x0000204000800080ULL, 0x0000200040008080ULL, 0x0000100020008080ULL, 0x0000080010008080ULL,
        0x0000040008008080ULL, 0x0000020004008080ULL, 0x0000800100020080ULL, 0x0000800041000080ULL,
        0x00FFFCDDFCED714AULL, 0x007FFCDDFCED714AULL, 0x003FFFCDFFD88096ULL, 0x0000040810002101ULL,
        0x0001000204080011ULL, 0x0001000204000801ULL, 0x0001000082000401ULL, 0x0001FFFAABFAD1A2ULL
    };

    // Bishop magics for each square (0-63)
    const Bitboard m_BishopMagics[64] = {
        0x0002020202020200ULL, 0x0002020202020000ULL, 0x0004010202000000ULL, 0x0004040080000000ULL,
        0x0001104000000000ULL, 0x0000821040000000ULL, 0x0000410410400000ULL, 0x0000104104104000ULL,
        0x0000040404040400ULL, 0x0000020202020200ULL, 0x0000040102020000ULL, 0x0000040400800000ULL,
        0x0000011040000000ULL, 0x0000008210400000ULL, 0x0000004104104000ULL, 0x0000002082082000ULL,
        0x0004000808080800ULL, 0x0002000404040400ULL, 0x0001000202020200ULL, 0x0000800802004000ULL,
        0x0000800400A00000ULL, 0x0000200100884000ULL, 0x0000400082082000ULL, 0x0000200041041000ULL,
        0x0002080010101000ULL, 0x0001040008080800ULL, 0x0000208004010400ULL, 0x0000404004010200ULL,
        0x0000840000802000ULL, 0x0000404002011000ULL, 0x0000808001041000ULL, 0x0000404000820800ULL,
        0x0001041000202000ULL, 0x0000820800101000ULL, 0x0000104400080800ULL, 0x0000020080080080ULL,
        0x0000404040040100ULL, 0x0000808100020100ULL, 0x0001010100020800ULL, 0x0000808080010400ULL,
        0x0000820820004000ULL, 0x0000410410002000ULL, 0x0000082088001000ULL, 0x0000002011000800ULL,
        0x0000080100400400ULL, 0x0001010101000200ULL, 0x0002020202000400ULL, 0x0001010101000200ULL,
        0x0000410410400000ULL, 0x0000208208200000ULL, 0x0000002084100000ULL, 0x0000000020880000ULL,
        0x0000001002020000ULL, 0x0000040408020000ULL, 0x0004040404040000ULL, 0x0002020202020000ULL,
        0x0000104104104000ULL, 0x0000002082082000ULL, 0x0000000020841000ULL, 0x0000000000208800ULL,
        0x0000000010011000ULL, 0x0000000008040800ULL, 0x0000000002040800ULL, 0x0000000001010000ULL
    };

    // Number of bits to shift = 64 - number of relevant blocking squares
    const int m_RookShifts[64] = {
        52, 53, 53, 53, 53, 53, 53, 52,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        52, 53, 53, 53, 53, 53, 53, 52
    };

    const int m_BishopShifts[64] = {
        58, 59, 59, 59, 59, 59, 59, 58,
        59, 59, 59, 59, 59, 59, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 59, 59, 59, 59, 59, 59,
        58, 59, 59, 59, 59, 59, 59, 58
    };

    Bitboard m_RookMasks[64];  
    Bitboard m_BishopMasks[64];

    int m_RookOffsets[64];
    int m_BishopOffsets[64];

    static Bitboard RookAttackTable[102400]; // 102400 is total occupancy variations of rook attacks
    static Bitboard BishopAttackTable[5248]; // 5248 is total occupancy variations of bishop attacks

    void initRookMasks();
    void initBishopMasks();
    void initSlidingOffsets();
    void initRookAttackTable();
    void initBishopAttackTable();
    Bitboard indexToOccupancy(int index, Bitboard mask) const {
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
    Bitboard generateRookAttacksSlow(int square, Bitboard occupied) const;
    Bitboard generateBishopAttacksSlow(int square, Bitboard occupied) const;
    void insertRookMoves(const BBPosition& position, std::vector<Move>& moves, Bitboard alliedPieces, Bitboard enemyPieces) const;
    void insertBishopMoves(const BBPosition& position, std::vector<Move>& moves, Bitboard alliedPieces, Bitboard enemyPieces) const;
  
    MoveGenerator() {
        initializeTables(); 
    }
    void initializeTables();
public:
    static MoveGenerator* getInstance() {
        if (s_instance == nullptr) {
            s_instance = new MoveGenerator();
        }
        return s_instance;
    }
    std::vector<Move> generateMoves(const BBPosition& position) const;
};
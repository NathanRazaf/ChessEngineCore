#include "BBPosition.h"
#include "Piece.h"
#include <map>
#include "MoveGenerator.h"


BBPosition::BBPosition() {
    clearBoard();
    m_isWhiteTurn = true;
}

BBPosition::BBPosition(const std::string& fen) {
    clearBoard();
    setPosition(fen);
}

void BBPosition::doMove(Square start, Square end) {
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

void BBPosition::setPosition(const std::string& fen) {
    // Map to find the piece types based on the letter read
    std::map<char, Type> pieceTypes = {
        {'p', PAWN},
        {'r', ROOK},
        {'n', KNIGHT},
        {'b', BISHOP},
        {'q', QUEEN},
        {'k', KING}
    };

    int rank = 7;  // Start at rank 8 (index 7)
    int file = 0;  // Start at file a (index 0)
    int index = 0;

    // Initialize position
    while (index < fen.length() && fen[index] != ' ') {
        char letter = fen[index]; // Current FEN letter to read
        if (fen[index] == '/') {
            rank--;      // Move to next rank down
            file = 0;    // Reset to file a
            index++;
            continue;
        }
        if (std::isdigit(letter)) {
            // If it's a digit, we skip that amount of squares because they're all empty
            int skip = letter - '0';
            file += skip;
        }
        else {
            // It's a piece
            // Uppercase letters are for white pieces
            Color color = std::isupper(letter) ? WHITE : BLACK;
            Type type = pieceTypes[std::tolower(letter)];
            uint8_t piece = createPiece(color, type);

            int square = rank * 8 + file;
            board[square] = piece;

			// Update the bitboard for this piece
			setBit(m_pieceBitboards[piece], square);
            file++;
        }
        index++; // Next FEN letter
    }

    index++; // Skip the space

    char color = fen[index];
    m_isWhiteTurn = color == 'w'; // White's turn if letter is w

    index += 2; // Skip the space and get directly to next FEN letter

    // Manage the castling rights
    bool has_K = false, has_Q = false, has_k = false, has_q = false;

    // Scan what castling rights are present
    while (index < fen.length() && fen[index] != ' ') {
        char right = fen[index];
        if (right == 'K') has_K = true;
        else if (right == 'Q') has_Q = true;
        else if (right == 'k') has_k = true;
        else if (right == 'q') has_q = true;
        index++;
    }

	// Set castling rights based on flags
	cr = 0;
	if (has_K) addCr(W_KING_SIDE);
	if (has_Q) addCr(W_QUEEN_SIDE);
	if (has_k) addCr(B_KING_SIDE);
	if (has_q) addCr(B_QUEEN_SIDE);

    index++; // Skip

    if (fen[index] != '-') { // It's an en passant move
        char file = fen[index];
        index++;
        char rank = fen[index];
        std::string strSquare = { file, rank };
        Square targetSquare = getSquareIndex(strSquare);
		// Set the en passant target square
		m_enPassantSquare = targetSquare;
    }
    else {
		m_enPassantSquare = EMPTY;
    }

    index += 2; // Skip space

    std::string halfmoveStr = "";
    // This is supposed to be the number representing the halfmove clock
    while (index < fen.length() && fen[index] != ' ') {
        halfmoveStr += fen[index];
        index++;
    }

    m_halfmoveClock = std::stoi(halfmoveStr);

    index++; // Skip space

    std::string fullmoveStr = "";
    // This is supposed to be the number representing the number of fullmoves
    while (index < fen.length() && fen[index] != ' ') {
        fullmoveStr += fen[index];
        index++;
    }

    m_fullmoves = std::stoi(fullmoveStr);
}

void BBPosition::clearBoard() {
    std::fill(m_pieceBitboards, m_pieceBitboards + 16, 0);
    std::fill(board, board + 64, 0);

    cr = 0;

	m_enPassantSquare = EMPTY;
}

Bitboard BBPosition::getOccupancyBitboard() const {
    Bitboard occupancy = 0;
    for (int i = 0; i < 16; i++) {
        occupancy |= m_pieceBitboards[i];
    }
	return occupancy;
}

Bitboard BBPosition::getColorBitboard(Color color) const {
    Bitboard occupancy = 0;
    // The first bitboards are the white pieces, the rest are the black pieces
    int start = 8 * (color == BLACK);
    int end = start + 8;
    for (int i = start; i < end; i++) {
        occupancy |= m_pieceBitboards[i];
    }
    return occupancy;
}

Piece BBPosition::getPiece(Square square) const {
	return board[square];
}
void BBPosition::setPiece(Square square, Piece piece) {
	board[square] = piece;
    
    if (piece != EMPTY) {
        // Set the square in the correct piece bitboard
        setBit(m_pieceBitboards[piece], square);
	}
}

std::vector<Move> BBPosition::getMoves() const {
    return MoveGenerator::getInstance()->generateMoves(*this);
}
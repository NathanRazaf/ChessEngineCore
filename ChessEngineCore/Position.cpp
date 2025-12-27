#include "Position.h"
#include <map>
#include <cctype>

Position::Position() {
    clear_board();
    is_white_turn = true;
}

Position::Position(const std::string& fen) {
    clear_board();
    set_position(fen);
}

Position::Position(const Position& other) {
    std::copy(other.board, other.board + 64, board);
    is_white_turn = other.is_white_turn;
}

void Position::move(int start, int end) {
    if (!is_square_valid(start)) {
        throw std::out_of_range("Start square index out of bounds");
    }
    if (!is_square_valid(end)) {
        throw std::out_of_range("End square index out of bounds");
    }
    uint8_t piece = board[start];
    board[end] = piece;
    board[start] = 0;

    // Swap turns
    is_white_turn = !is_white_turn;
}

uint8_t Position::get_piece(int square) const {
    if (!is_square_valid(square)) {
        throw std::out_of_range("Square index out of bounds");
    }
    return board[square];
}

void Position::set_piece(int square, uint8_t piece) {
    if (!is_square_valid(square)) {
        throw std::out_of_range("Square index out of bounds");
    }
    board[square] = piece;
};


void Position::set_position(const std::string& fen) {
    // Map to find the piece types based on the letter read
    std::map<char, Type> pieceTypes = {
        {'p', Type::PAWN},
        {'r', Type::ROOK},
        {'n', Type::KNIGHT},
        {'b', Type::BISHOP},
        {'q', Type::QUEEN},
        {'k', Type::KING}
    };
   
    int square = 0;
    int index = 0;

    // Initialize position
    while (index < fen.length() && fen[index] != ' ') {
        char letter = fen[index]; // Current FEN letter to read
        if (fen[index] == '/') {
            index++;
            continue; // It's to get to next line, we use a 1-dimensional array
                      // so we don't need to process it
        }
        if (std::isdigit(letter)) {
            // If it's a digit, we skip that amount of squares because they're all empty
            int skip = letter - '0';
            square += skip;
        }
        else {
            // It's a piece
            // Uppercase letters are for white pieces
            Color color = std::isupper(letter) ? Color::WHITE : Color::BLACK;
            Type type = pieceTypes[std::tolower(letter)];
            uint8_t piece = Piece::create_piece(color, type);

            board[square] = piece;
            square++;
        }
        index++; // Next FEN letter
    }

    index++; // Skip the space

    char color = fen[index];
    is_white_turn = color == 'w'; // White's turn if letter is w

    // TODO : Treat en-passant state and castling state
}

std::vector<int> Position::get_sliding_moves(uint8_t piece, int square) const {
    if (!is_square_valid(square)) {
        throw std::out_of_range("End square index out of bounds");
    }

    int ROOK_MOVES[4]   = { -8, -1, 1, 8 };
    int BISHOP_MOVES[4] = { -9, -7, 7, 9 };
    int QUEEN_MOVES[8]  = { -9, -8, -7, -1, 1, 7, 8, 9 };
    
    Type piece_type = Piece::get_type(piece);
    const int* move_dirs;
    int dir_count;

    switch (piece_type) {
    case Type::ROOK:
        move_dirs = ROOK_MOVES;
        dir_count = 4;
        break;
    case Type::BISHOP:
        move_dirs = BISHOP_MOVES;
        dir_count = 4;
        break;
    case Type::QUEEN:
        move_dirs = QUEEN_MOVES;
        dir_count = 8;
        break;
    default:
        return {}; // It's supposed to be a sliding piece
    }

    std::vector<int> moves = {};
    
    for (int i = 0; i < dir_count; i++) {
        int current_square = square;
        int direction = move_dirs[i];
    }

    return moves;
}

std::vector<int> Position::get_moves(int square) const {
    return {};
}

void Position::clear_board() {
    std::fill(board, board + 64, 0);
}
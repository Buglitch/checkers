#include "board.hh"

#include "error.hh"

Board::Board()
{
    struct pos p;
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            this->set_piece(EMPTY_PIECE, p);
            if ((p.x + p.y) % 2 == 0)
                this->set_bit(INVALID_POS, p);
            else if (p.y + 1 < BOARD_DIMENSION / 2) {
                this->set_bit(BLACK_PIECE, p);
            } else if (p.y > BOARD_DIMENSION / 2) {
                this->set_bit(WHITE_PIECE, p);
            }
        }
    }
}

inline std::size_t coordinate_to_index(struct pos p)
{
    return p.y * BOARD_DIMENSION + p.x;
}

piece_t Board::get_piece(struct pos p) const
{
    if (!in_range(p))
        handle_error(ERR_OUT_OF_BOUND);

    return this->board[coordinate_to_index(p)];
}

void Board::set_piece(piece_t bitmap, struct pos p)
{
    this->board[coordinate_to_index(p)] = bitmap;
}

void Board::set_bit(piece_t bitmap, struct pos p)
{
    this->set_piece(this->get_piece(p) | bitmap, p);
}

void Board::clear_bit(piece_t bitmap, struct pos p)
{
    this->set_piece(this->get_piece(p) & ~((piece_t)bitmap), p);
}

void Board::clear_all(piece_t bitmap)
{
    struct pos p;
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            this->clear_bit(bitmap, p);
        }
    }
}

void Board::empty_all(piece_t bitmap)
{
    struct pos p;
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            if (this->get_piece(p) & bitmap)
                this->set_piece((piece_t)EMPTY_PIECE, p);
        }
    }
}

std::ostream &operator<<(std::ostream &os, Board &board)
{
    struct pos p;
#ifndef SOUFLER
    bool capture_bit = false;
#endif
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            piece_t bitmap = board.get_piece(p);
            if (bitmap & HASH_MARKER)
                os << "#";
            else if (bitmap & INVALID_POS)
                os << ".";
            else if ((bitmap & PIECE_TYPE) == EMPTY_PIECE)
                os << " ";
            else if (bitmap & WHITE_PIECE && bitmap & KG_MODIFIER)
                os << "O";
            else if (bitmap & WHITE_PIECE)
                os << "o";
            else if (bitmap & BLACK_PIECE && bitmap & KG_MODIFIER)
                os << "X";
            else if (bitmap & BLACK_PIECE)
                os << "x";
            else
                os << "E";

            if (bitmap & IS_CAPTURED)
                os << "]";
            else if (bitmap & IS_SELECTED)
                os << "*";
            else if (bitmap & CAPTURE_BIT)
                os << "E";
            else
                os << " ";

#ifndef SOUFLER
            if (bitmap & CAPTURE_BIT)
                capture_bit = true;
#endif
        }
        os << std::endl;
    }
#ifndef SOUFLER
    if (capture_bit) {
        for (size_t i = 0; i < BOARD_DIMENSION; i++)
            os << "  ";
        os << ">" << std::endl;
    }
#endif
    return os;
}

piece_t get_opposite_color(piece_t piece)
{
    piece_t rest = piece & ~PIECE_TYPE;
    piece_t type = piece & PIECE_TYPE;
    if (type == WHITE_PIECE)
        rest |= BLACK_PIECE;
    else
        rest |= WHITE_PIECE;
    return rest;
}

bool in_range(struct pos p)
{
    return (p.x < BOARD_DIMENSION && p.y < BOARD_DIMENSION);
}

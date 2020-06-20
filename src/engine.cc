#include "engine.hh"

#include "error.hh"

Game::Game()
{
    this->board = new Board();
    this->state = WHITE_TURN;
}

void Game::play(struct pos p)
{
    static moves_t moves;

#ifndef SOUFLER
    if (p.x == BOARD_DIMENSION && p.y == BOARD_DIMENSION && !moves.empty() &&
        (this->board->get_piece(moves.front().src) & CAPTURE_BIT))
        force_next_turn();
#endif

    if (!in_range(p))
        return;

    piece_t piece = this->board->get_piece(p);
    if (piece & INVALID_POS)
        return;

    if (piece & HASH_MARKER) {
        for (auto it = moves.begin(); it != moves.end(); it++) {
            if (it->dst.x == p.x && it->dst.y == p.y) {
                apply_move(*it);
                moves = get_moves(p);
                display_moves(moves);
                break;
            }
        }
    } else if ((piece & PIECE_TYPE) == EMPTY_PIECE) {
        return;
    } else {
        piece_t piece_type;

        if (this->state == WHITE_TURN)
            piece_type = WHITE_PIECE;
        else
            piece_type = BLACK_PIECE;

        if ((piece & PIECE_TYPE) == piece_type) {
            moves_t moves_tmp = get_moves(p);
            if (!moves_tmp.empty()) {
                moves = moves_tmp;
                unselect_all();
                this->board->set_bit(IS_SELECTED, p);
                display_moves(moves);
            }
        }
    }
}

// States

void Game::next_turn(struct move move)
{
    if (!move.capture || get_moves(move.dst).empty()) {
        this->force_next_turn();
    }
}

void Game::force_next_turn()
{
    unselect_all();
    kill_all_captured();
    this->board->clear_all(CAPTURE_BIT);
    if (state == WHITE_TURN) {
        state = BLACK_TURN;
    } else if (state == BLACK_TURN) {
        state = WHITE_TURN;
    }
}

void Game::end_game()
{
    size_t white_count = 0;
    size_t black_count = 0;
    struct pos p;
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            piece_t piece = this->board->get_piece(p);
            if ((piece & PIECE_TYPE) == WHITE_PIECE)
                white_count++;
            else if ((piece & PIECE_TYPE) == BLACK_PIECE)
                black_count++;
        }
    }
    if (white_count > black_count)
        this->state = WHITE_WIN;
    else if (black_count > white_count)
        this->state = BLACK_WIN;
    else
        this->state = DRAW;
}

bool Game::is_running()
{
    return (this->state == WHITE_TURN || this->state == BLACK_TURN);
}

// Moves

moves_t Game::get_moves(struct pos p)
{
    if (!in_range(p))
        return moves_t();

    moves_t board_moves = get_all_board_moves();

    if (board_moves.empty()) {
        this->end_game();
        return board_moves;
    }

    bool capture_bit = false;
    for (auto it = board_moves.begin(); it != board_moves.end() && !capture_bit;
         it++) {
        if (this->board->get_piece(it->src) & CAPTURE_BIT)
            capture_bit = true;
    }
    moves_t moves = moves_t();
#ifdef SOUFLER
    bool soufler = false;
    for (auto it = board_moves.begin(); it != board_moves.end() && !soufler;
         it++) {
        if (it->capture)
            soufler = true;
    }
    for (auto it = board_moves.begin(); it != board_moves.end(); it++) {
        if (p.x == it->src.x && p.y == it->src.y &&
            (!capture_bit || this->board->get_piece(it->src) & CAPTURE_BIT) &&
            (!soufler || it->capture))
            moves.push_back(std::move(*it));
    }
#else
    for (auto it = board_moves.begin(); it != board_moves.end(); it++) {
        if (p.x == it->src.x && p.y == it->src.y &&
            (!capture_bit || this->board->get_piece(it->src) & CAPTURE_BIT))
            moves.push_back(std::move(*it));
    }
#endif
    return moves;
}

moves_t Game::get_all_moves(struct pos p)
{
    moves_t moves = moves_t();
    piece_t piece = this->board->get_piece(p);
    if (piece & INVALID_POS)
        return moves;

    if (((piece & PIECE_TYPE) == WHITE_PIECE && this->state == WHITE_TURN) ||
        ((piece & PIECE_TYPE) == BLACK_PIECE && this->state == BLACK_TURN)) {
        piece_t piece_type;
        int y_direction;
        if (this->state == WHITE_TURN) {
            piece_type = WHITE_PIECE;
            y_direction = -1;
        } else {
            piece_type = BLACK_PIECE;
            y_direction = +1;
        }

        // walking
        if (!(this->board->get_piece(p) & CAPTURE_BIT)) {
            if (piece & KG_MODIFIER) {
                // king
                for (int j = -1; j <= 1; j += 2) {
                    for (int k = -1; k <= 1; k += 2) {
                        int i = 1;
                        while (true) {
                            struct move move;
                            move.src = p;
                            move.dst.x = move.src.x + i * k;
                            move.dst.y = move.src.y + i * j;
                            move.capture = false;
                            if (!in_range(move.dst))
                                break;
                            if ((this->board->get_piece(move.dst) &
                                 PIECE_TYPE) == EMPTY_PIECE)
                                moves.push_back(std::move(move));
                            else
                                break;
                            i++;
                        }
                    }
                }
            } else {
                // pawn
                for (int i = -1; i <= 1; i += 2) {
                    struct move move;
                    move.src = p;
                    move.dst.x = move.src.x + i;
                    move.dst.y = move.src.y + y_direction;
                    move.capture = false;
                    if (in_range(move.dst))
                        if ((this->board->get_piece(move.dst) & PIECE_TYPE) ==
                            EMPTY_PIECE)
                            moves.push_back(std::move(move));
                }
            }
        }

        // eating
        for (int i = -2; i <= 2; i += 4) {
            for (int j = -2; j <= 2; j += 4) {
                struct move move;
                move.src = p;
                move.dst.x = move.src.x + j;
                move.dst.y = move.src.y + i;
                move.capture = true;
                move.captured.x = move.src.x + (j / 2);
                move.captured.y = move.src.y + (i / 2);
                if (in_range(move.dst))
                    if ((this->board->get_piece(move.dst) & PIECE_TYPE) ==
                        EMPTY_PIECE)
                        if ((this->board->get_piece(move.captured) &
                             PIECE_TYPE) == get_opposite_color(piece_type) &&
                            !(this->board->get_piece(move.captured) &
                              IS_CAPTURED))
                            moves.push_back(std::move(move));
            }
        }
    }

    return moves;
}

moves_t Game::get_all_board_moves()
{
    moves_t moves = moves_t();
    struct pos p;
    for (p.y = 0; p.y < BOARD_DIMENSION; p.y++) {
        for (p.x = 0; p.x < BOARD_DIMENSION; p.x++) {
            moves.splice(moves.end(), get_all_moves(p));
        }
    }
    return moves;
}

void Game::display_moves(moves_t moves)
{
    for (auto it = moves.begin(); it != moves.end(); it++) {
        this->board->set_bit(HASH_MARKER, it->dst);
    }
}

void Game::apply_move(struct move move)
{
    this->board->set_piece(this->board->get_piece(move.src), move.dst);
    this->board->set_piece((piece_t)EMPTY_PIECE, move.src);
    this->board->clear_all(HASH_MARKER);
    if (move.capture) {
        this->board->set_bit(IS_CAPTURED, move.captured);
        this->board->set_bit(CAPTURE_BIT, move.dst);
    }
    if (((this->board->get_piece(move.dst) & PIECE_TYPE) == WHITE_PIECE &&
         move.dst.y == 0) ||
        ((this->board->get_piece(move.dst) & PIECE_TYPE) == BLACK_PIECE &&
         move.dst.y == BOARD_DIMENSION - 1))
        this->board->set_bit(KG_MODIFIER, move.dst);
    this->next_turn(move);
}

// Board

void Game::unselect_all()
{
    this->board->clear_all(IS_SELECTED);
    this->board->clear_all(HASH_MARKER);
}

void Game::kill_all_captured()
{
    this->board->empty_all(IS_CAPTURED);
}

// Display

std::ostream &operator<<(std::ostream &os, Game &game)
{
    os << "Game State: ";
    switch (game.state) {
    case WHITE_TURN:
        os << "White Turn";
        break;
    case BLACK_TURN:
        os << "Black Turn";
        break;
    case WHITE_WIN:
        os << "White Player Won!";
        break;
    case BLACK_WIN:
        os << "Black Player Won!";
        break;
    case DRAW:
        os << "Draw!";
        break;
    }
    os << std::endl;
    if (game.board)
        os << *game.board;
    else
        handle_error(ERR_NULL_POINTER);
    return os;
}

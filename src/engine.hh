#pragma once

#include <iostream>

#include "board.hh"
#include "move.hh"

enum GameState { WHITE_TURN, BLACK_TURN, WHITE_WIN, BLACK_WIN, DRAW };

class Game {
  private:
    Board *board;
    GameState state;

  public:
    Game();
    void play(struct pos p);
    // States
    void next_turn(struct move move);
    void force_next_turn();
    void end_game();
    bool is_running();
    // Moves
    moves_t get_moves(struct pos p);
    moves_t get_all_moves(struct pos p);
    moves_t get_all_board_moves();
    void display_moves(moves_t moves);
    void apply_move(struct move move);
    // Board
    void unselect_all();
    void kill_all_captured();
    // Display
    friend std::ostream &operator<<(std::ostream &os, Game &board);
};

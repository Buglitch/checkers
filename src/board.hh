#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>

/* piece_t bitmap *
================================

      marker      black piece
      |  capture  | white piece
      |  |        |  |
      v  v        v  v
7  6  5  4  3  2  1  0
^  ^        ^  ^
|  |        |  |
|  selected |  king
invalid     dead

--------------------------------

                  empty
                  |  |
                  v  v
0  X  X  X  X  X  0  0
^
valid position

*/

#define EMPTY_PIECE 0x0
#define WHITE_PIECE 0x1 << 0
#define BLACK_PIECE 0x1 << 1
#define KG_MODIFIER 0x1 << 2
#define IS_CAPTURED 0x1 << 3
#define CAPTURE_BIT 0x1 << 4
#define HASH_MARKER 0x1 << 5
#define IS_SELECTED 0x1 << 6
#define INVALID_POS 0x1 << 7

#define PIECE_TYPE 0x3

#ifndef BOARD_DIMENSION
#define BOARD_DIMENSION 10
#endif

typedef uint8_t piece_t;
typedef piece_t board_t[BOARD_DIMENSION * BOARD_DIMENSION];

struct pos {
    std::size_t x;
    std::size_t y;
};

class Board {
  private:
    board_t board;

  public:
    Board();
    piece_t get_piece(struct pos p) const;
    void set_piece(piece_t bitmap, struct pos p);
    void set_bit(piece_t bitmap, struct pos p);
    void clear_bit(piece_t bitmap, struct pos p);
    void clear_all(piece_t bitmap);
    void empty_all(piece_t bitmap);
    friend std::ostream &operator<<(std::ostream &os, Board &board);
};

piece_t get_opposite_color(piece_t piece);
bool in_range(struct pos p);

#pragma once

#include <list>

#include "board.hh"

struct move {
    struct pos src;
    struct pos dst;
    bool capture;
    struct pos captured;
};

typedef std::list<struct move> moves_t;

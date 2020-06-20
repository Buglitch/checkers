#pragma once

#include <cstddef>

#define ERR_NONE 0
#define ERR_GENERIC 1
#define ERR_OUT_OF_BOUND 2
#define ERR_NULL_POINTER 3

void handle_error(std::size_t code);

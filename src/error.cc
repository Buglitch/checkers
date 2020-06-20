#include "error.hh"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

static void warnx(const char *format, va_list args)
{
    vfprintf(stderr, format, args);
}

static void errx(int status, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    warnx(format, args);
    va_end(args);
    exit(status);
}

void handle_error(std::size_t code)
{
    switch (code) {
    case ERR_NONE:
        return;
    case ERR_GENERIC:
        errx(code, "Error\n");
    case ERR_OUT_OF_BOUND:
        errx(code, "Error: out of bound\n");
    case ERR_NULL_POINTER:
        errx(code, "Error: null pointer\n");
    }
    errx(code, "Error: e%02d\n", code);
}

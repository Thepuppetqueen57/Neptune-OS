#ifndef TERMINAL_H
#define TERMINAL_H

#include "../lib/seqft/common.h"

extern void highlight_error(const char* expr,
                            size_t      expr_len,
                            IterErr     error,
                            size_t      indent);

extern int osmain(int* processes, int maxprocesses, int maxthreadsperprocess);

#endif // TERMINAL_H
#ifndef __COMMON_H
#define __COMMON_H

#include <stdbool.h>

#define SCREEN_HEIGHT 15
#define SCREEN_WIDTH 97
// WORD_AMOUNT should be at least 5 less than SCREEN_HEIGHT, to accomodate interface
// elements.
#define WORD_AMOUNT 10

#define _POSIX_C_SOURCE 200809L

#define UNUSED(x) (void)(x)

#endif

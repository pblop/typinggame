#ifndef __TERM_OUTPUT_H
#define __TERM_OUTPUT_H

#define CSI "\x1B["
#define GOTO CSI"%d;%dH"
#define GOTO_HOME CSI"H"
#define CLEAR CSI"2J"
#define HIDE_CURSOR CSI"?25l"
#define SHOW_CURSOR CSI"?25h"
#define RGB_COLOUR CSI"38;2;%d;%d;%dm"
#define REMOVE_COLOUR CSI"39m"
#define RGB_BACKGROUND CSI"48;2;%d;%d;%dm"
#define REMOVE_BACKGROUND CSI"49m"

typedef struct {
  unsigned char r, g, b;
} colour_t;

#endif

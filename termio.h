#ifndef __TERMIO_H
#define __TERMIO_H
#include <termios.h>

#define CSI "\x1b["
#define GOTO CSI"%d;%dH"
#define GOTO_HOME CSI"H"
#define CLEAR CSI"2J"
#define HIDE_CURSOR CSI"?25l"
#define SHOW_CURSOR CSI"?25h"
#define RGB_COLOUR CSI"38;2;%d;%d;%dm"
#define REMOVE_COLOUR CSI"39m"
#define RGB_BACKGROUND CSI"48;2;%d;%d;%dm"
#define REMOVE_BACKGROUND CSI"49m"

struct termios orig_screen_attrs;

typedef struct {
  unsigned char r, g, b;
} colour_t;

// KB HANDLING /*{{{*/
typedef enum
{
  KEY_NONE = -1,
  KEY_UP = 'U',
  KEY_DOWN = 'D',
  KEY_LEFT = 'L',
  KEY_RIGHT = 'R',
  KEY_W = 'w',
  KEY_A = 'a',
  KEY_S = 's',
  KEY_D = 'd'
} termkey_t;

int kbhit();
termkey_t get_user_input();
/*}}}*/

int setup_screen(void);
void unsetup_screen(void);

int ensure_screen_size(void);
int get_window_size(int *width, int *height);


#endif

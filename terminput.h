#ifndef __TERMINPUT_H
#define __TERMINPUT_H
#include <termios.h>

struct termios orig_screen_attrs;

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

int setup_screen(void);
void unsetup_screen(void);

#endif

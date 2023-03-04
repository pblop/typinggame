#include "terminput.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

// Copied this fn from
// https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
int kbhit()
{
  struct timeval tv = {0L, 0L};

  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv) > 0;
}
termkey_t get_user_input()
{
  int c;

  if (!kbhit())
    return KEY_NONE;

  if ((c = getc(stdin)) == EOF)
    return -12;

  switch (c)
  {
    case 'w':
      return KEY_W;
    case 'a':
      return KEY_A;
    case 's':
      return KEY_S;
    case 'd':
      return KEY_D;

    case 27: // ansi escape code for arrows
      if ((c = getc(stdin)) == EOF)
        return KEY_NONE;
      if (c == 91)// continuation of the ansi escape code for arrows
      {
        if ((c = getc(stdin)) == EOF)
          return KEY_NONE;

        switch (c)
        {
          case 'A':
            return KEY_UP;
          case 'B':
            return KEY_DOWN;
          case 'C':
            return KEY_RIGHT;
          case 'D':
            return KEY_LEFT;
        }
      }

      break;
  }


  return KEY_NONE;
}


int setup_screen(void)
{
  struct termios tattr;
  sigset_t empty_set;
  
  // set noncanonical input mode
  if (!isatty(STDIN_FILENO))
  {
    fprintf(stderr, "Not a terminal");
    return -1;
  }

  // Save the terminal attributes so we can restore them later.
  tcgetattr(STDIN_FILENO, &orig_screen_attrs);

  // Set the funny terminal modes.
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
  
  return 0;
}


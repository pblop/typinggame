#include "termio.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

// Copied this fn from
// https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
int kbhit()/*{{{*/
{
  struct timeval tv = {0L, 0L};

  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv) > 0;
}/*}}}*/
termkey_t get_user_input()/*{{{*/
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
}/*}}}*/

int ensure_screen_size(void)/*{{{*/
{
  int width, height, prevwidth, prevheight;

  prevwidth = 0, prevheight = 0;
  if (get_window_size(&width, &height) != 0)
    return -1;

  // if we are displaying a (WxH) 31x31 image we need 31x16 lines, I use
  // (x+1)/2 as a way to divide and ceil at the same time.
  while (width < SCREEN_WIDTH || height < (SCREEN_HEIGHT+1)/2)
  {
    if (prevwidth != width || prevheight != height)
    {
      printf(CLEAR GOTO
          "Screen size is too small.\n"
          "Please resize your terminal to at least %dx%d.\n"
          "The current size is %dx%d.\n",
          1, 1,
          SCREEN_WIDTH, SCREEN_HEIGHT, width, height);
      fflush(stdout);
    }

    prevwidth = width, prevheight = height;
    if (get_window_size(&width, &height) != 0)
      return -1;
  }

  return 0;
}/*}}}*/

int get_window_size(int *width, int *height)/*{{{*/
{
  struct winsize ws;
  if (ioctl(0, TIOCGWINSZ, &ws) != 0)
  {
    perror("ioctl");
    return -1;
  }

  *width = ws.ws_col;
  *height = ws.ws_row;
  return 0;
}/*}}}*/


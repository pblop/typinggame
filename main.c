#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "termio.h"
#include "util.h"
#include "common.h"

#define exitf(num) {unsetup_screen(); exit(num);}

#define BACKGROUND_COLOUR 50, 50, 200

void sigint_handler(int signum);
void setup_sigint_handler(void);
int draw_screen(game_t *game);
int handle_user_input(game_t *game, termkey_t pressed_key);

void sigint_handler(int signum)/*{{{*/
{
  unsetup_screen();
  exit(0);
}/*}}}*/
void setup_sigint_handler(void)/*{{{*/
{
  sigset_t empty_set;
  struct sigaction action;

  // Capture SIGINT and call unsetup_screen if program exited that way.
  sigemptyset(&empty_set);
  action.sa_handler = sigint_handler;
  action.sa_mask = empty_set;
  action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &action, NULL);
}/*}}}*/

int draw_screen(game_t *game)/*{{{*/
{
  printf(CLEAR REMOVE_COLOUR REMOVE_BACKGROUND GOTO_HOME
         RGB_BACKGROUND GOTO, BACKGROUND_COLOUR, SCREEN_HEIGHT, SCREEN_WIDTH);

  return 0;
}
/*}}}*/
int handle_user_input(game_t *game, termkey_t pressed_key)/*{{{*/
{
  if (game == NULL)
    return -1;

  switch (pressed_key)
  {
    default: // Do not do anything for the rest of the keys
      break;
  }

  return 0;
}
/*}}}*/

int main()
{
  game_t game;
  termkey_t user_key;

  /* Screen and exit setup */
  setup_screen();
  setup_sigint_handler();
  printf(HIDE_CURSOR CLEAR GOTO_HOME);

  /* Game stuff */
  if (init_game(&game) != 0)
    exitf(1);

  if (load_dict(&game, "words.txt") != 0)
    exitf(2);

  while(1) {
    if ((user_key = get_user_input()) != KEY_NONE)
    {
      if (handle_user_input(&game, user_key) != 0)
        exitf(3);
    }

    if (draw_screen(&game) != 0)
      exitf(4);

    game.frame++;
  }

  /* Finish */
  unsetup_screen();

  return 0;
}

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
int main_loop(game_t *game);

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
  // Fill the background with blue.
  printf(CLEAR REMOVE_COLOUR REMOVE_BACKGROUND GOTO_HOME
         RGB_BACKGROUND GOTO, BACKGROUND_COLOUR, SCREEN_HEIGHT, SCREEN_WIDTH);

  // TODO: Print each word character, in one colour if it has already been
  // typed, and in another one if it has not yet been typed.

  return 0;
}
/*}}}*/
int handle_user_input(game_t *game, termkey_t pressed_key)/*{{{*/
{
  if (game == NULL)
    return -1;

  // TODO: Go through each word's next not-typed character, and compare it
  // to the pressed key, if it is the pressed key, increment that word's
  // typedchars counter by one.
  // EXTRA TODO: Maybe only take into consideration characters that are visible,
  // because words start off the screen, and slowly appear.
  switch (pressed_key)
  {
    default: // Do not do anything for the rest of the keys
      break;
  }

  return 0;
}
/*}}}*/
int main_loop(game_t *game)/*{{{*/
{
  if (game == NULL)
    return -1;

  // TODO: Move every word one position to the left (increment x). Maybe make
  //       the word speed relative to the spent time, or the score.
  // TODO: Move every word that's finished (strlen(word.ptr) == word.typedchars)
  //       down (increment y).

  return 0;
}/*}}}*/

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

  fprintf(stderr, "Loading words...\n");
  if (load_dict(&game, "words.txt") != 0)
    exitf(2);
  fprintf(stderr, "Done!\n");

  for (int i = 0; i < 100; i++)
    fprintf(stderr, "%s\n", game.dict[i]);

  put_word_in_game(&game);
  for (int i = 0; i < SCREEN_HEIGHT; i++)
    fprintf(stderr, "%s", game.words[i].ptr);

  while (1);

  while(1) {
    if (ensure_screen_size() != 0)
      exitf(5);

    if ((user_key = get_user_input()) != KEY_NONE)
    {
      if (handle_user_input(&game, user_key) != 0)
        exitf(3);
    }

    if (main_loop(&game) != 0)
      exitf(6);

    if (draw_screen(&game) != 0)
      exitf(4);

    game.frame++;
    usleep(500*1000);
  }

  /* Finish */
  unsetup_screen();

  return 0;
}

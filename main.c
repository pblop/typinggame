#include "common.h"
#include "termio.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define exitf(num) {unsetup_screen(); exit(num);}

#define FPS 120
#define every_secs(g, n) ((g)->frame % ((int) (FPS * n)) == 0)

#define BACKGROUND_COLOUR 150, 150, 255
#define UNTYPED_CHAR_COLOUR 50, 50, 50
#define TYPED_CHAR_COLOUR 250, 250, 250
#define INTERFACE_COLOUR 10, 10, 10 

void sigint_handler(int signum);
void setup_sigint_handler(void);
int draw_screen(game_t *game);
int handle_user_input(game_t *game, termkey_t pressed_key);
int main_loop(game_t *game);

void sigint_handler(int signum)/*{{{*/
{
  UNUSED(signum);
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
  // Clear everything. New frame.
  printf(REMOVE_COLOUR REMOVE_BACKGROUND CLEAR);
  // Draw the background.
  printf(RGB_BACKGROUND, BACKGROUND_COLOUR);
  for (int y = 1; y <= SCREEN_HEIGHT; y++)
    printf(GOTO CLEAR_LINE_TO_START, y, SCREEN_WIDTH);

  // Print each word character, in one colour if it has already been
  // typed, and in another one if it has not yet been typed.
  // TODO: Draw words centered (if there are less words than the height of the
  // screen, have the same number of spaces on the top and the bottom).
  int selected_word = get_selected_word_i(game);/*{{{*/
  for (int k = 0; k < 2; k++)
  {
    for (int i = 0; i < WORD_AMOUNT; i++)
    {
      scrword_t *word = &game->words[i];
      if (word->ptr == NULL)
        continue; // No word.

      // Print the finished words first, and then the unfinished ones on top,
      // so that the unfinished ones are not hidden by the finished ones.
      if (k == 0 && !is_word_finished(word))
        continue;
      if (k == 1 && is_word_finished(word))
        continue;

      int x = word->x;
      int y = word->y;
      int len = strlen(word->ptr);

      // Only display x characters (because only x characters are shown on the
      // screen). And at most len characters (the length of the word).
      for (int j = 0; j < x && j < len; j++)
      {
        if (SCREEN_WIDTH - (x - j) < 1)
          continue; // The character is not visible.

        if (i == selected_word)
          printf(UNDERLINE_ON);
        if (j < word->typedchars)
          printf(GOTO RGB_COLOUR, i+1+y, SCREEN_WIDTH - (x - j), TYPED_CHAR_COLOUR);
        else
          printf(GOTO RGB_COLOUR, i+1+y, SCREEN_WIDTH - (x - j), UNTYPED_CHAR_COLOUR);
        printf("%c", word->ptr[j]);
      }

      if (i == selected_word)
        printf(UNDERLINE_OFF);
    }
  }
/*}}}*/

  printf(GOTO RGB_COLOUR "Score: %d", WORD_AMOUNT+1, 5, INTERFACE_COLOUR, game->score);
  printf(GOTO RGB_COLOUR "Lives: %d", WORD_AMOUNT+2, 5, INTERFACE_COLOUR, game->lives);
  printf(GOTO RGB_COLOUR "[d] Word appearance speed: %.3f words/s", WORD_AMOUNT+3, 5, INTERFACE_COLOUR, 1/word_appearance_speed(game));
  printf(GOTO RGB_COLOUR "[d] Word speed: %.3f pix/s", WORD_AMOUNT+4, 5, INTERFACE_COLOUR, 1/word_move_speed(game));

  fflush(stdout);
  return 0;
}
/*}}}*/
int handle_user_input(game_t *game, termkey_t pressed_key)/*{{{*/
{
  if (game == NULL)
    return -1;

  // Go through the selected word's next not-typed character, and compare it
  // to the pressed key, if it is the pressed key, increment that word's
  // typedchars counter by one.
  int selected_word_i = get_selected_word_i(game);
  if (selected_word_i == -1)
    return -2;

  scrword_t *word = &game->words[selected_word_i];
  if (word->ptr == NULL)
    return -3; // No word.

  int x = word->x;
  int len = strlen(word->ptr);

  if (is_word_finished(word))
    return -4; // Word is already typed.

  // We're only displaying x characters. So we're only interested in the
  // first x characters (or at most the length of the word).
  // NOTE: Checking only the first x characters may not be needed now that
  // the selected word is handled by a different function.
  if (word->typedchars >= x || word->typedchars == len)
    return 0; // No more characters to type.
 
  if (word->ptr[word->typedchars] == pressed_key)
    word->typedchars++;

  return 0;
}
/*}}}*/
int main_loop(game_t *game)/*{{{*/
{
  if (game == NULL)
    return -1;

  // Move every word one position to the left (increment x). Maybe make
  // the word speed relative to the spent time, or the score.

  // Make the speed the words appear at and the speed they move at
  // relative to the score.

  if (every_secs(game, word_move_speed(game))) {
    for (int i = 0; i < WORD_AMOUNT; i++)
    {
      scrword_t *word = &game->words[i];
      if (word->ptr == NULL)
        continue; // No word.

      if (!is_word_finished(word))
        word->x++;
    }
  }

  // Move every word that's finished (strlen(word.ptr) == word.typedchars)
  // down (increment y).
  if (every_secs(game, 0.5))
    for (int i = 0; i < WORD_AMOUNT; i++)
    {
      scrword_t *word = &game->words[i];
      if (word->ptr == NULL)
        continue; // No word.

      if (is_word_finished(word))
      {
        // If this is the first time we're moving it, add its point value to
        // the score. (We must only do it once, so doing it on the first
        // occasion we move it is a great place to do it)
        if (word->y == 0)
          game->score += word->length;

        word->y++;
      }
    }

  for (int i = 0; i < WORD_AMOUNT; i++)
  {
    scrword_t *word = &game->words[i];
    if (word->ptr == NULL)
      continue; // No word.

    if (word->y+i+1 >= SCREEN_HEIGHT)
    {
      // Word has reached the bottom of the screen. Remove it.
      clear_word(game, i);
    }
    // Lose a life when an untyped character hits the border of the screen
    if (word->x-word->typedchars >= SCREEN_WIDTH)
    {
      // Word has hit the left of the screen. Remove it.
      clear_word(game, i);
      game->lives--;
    }
  }

  if (every_secs(game, word_appearance_speed(game)))
    put_word_in_game(game);

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

  while(1) {
    if (ensure_screen_size() != 0)
      exitf(5);

    start_frame();

    if ((user_key = get_user_input()) != KEY_NONE)
    {
      if (handle_user_input(&game, user_key) != 0)
        exitf(3);
    }

    if (main_loop(&game) != 0)
      exitf(6);

    if (game.lives <= 0)
      break;

    if (every_secs(&game, 0.2))
      if (draw_screen(&game) != 0)
        exitf(4);

    game.frame++;
    ensure_frame_time(1000 / FPS);
  }

  /* Finish */
  unsetup_screen();

  return 0;
}

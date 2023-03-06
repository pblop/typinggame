#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

#define log1(str) fprintf(stderr, str"\n")
#define log(str, ...) fprintf(stderr, str"\n", __VA_ARGS__)

int init_game(game_t* game)/*{{{*/
{
  if (game == NULL)
    return -1;

  game->score = 0;
  game->lives = 3; // The initial number of lives.
  game->dictsize = -1;
  game->dict = NULL;
  game->frame = 0;

  srand(time(NULL));

  // Init words.
  for (int i = 0; i < SCREEN_HEIGHT; i++)
    game->words[i] = (scrword_t) {NULL, 0, 0, 0, 0, 0};

  return 0;
}
/*}}}*/

char* mall_fword(FILE* fp)/*{{{*/
{
  // Move the file pointer until we find a word end ('\n' or EOF),
  // then go back to the initial location and allocate and fill that string.

  char c;
  long pos = ftell(fp);
  if (pos < 0)
    return NULL;

  // Count the length of the word
  int word_len = 0;
  while (1)
  {
    c = getc(fp);
    if (c == '\n' || c == EOF)
    {
      break;
    }
    else {
      word_len++;
    }
  }

  // Go back to the start of the word
  if (fseek(fp, pos, SEEK_SET) != 0)
    return NULL;

  // Allocate the word
  char* word = malloc((word_len+1) * sizeof(char));
  if (word == NULL)
    return NULL;

  // Fill the word
  for (int i = 0; i < word_len; i++)
    word[i] = fgetc(fp);
  fgetc(fp); // Skip the newline
  word[word_len] = '\0';

  return word;
}
/*}}}*/
int load_dict(game_t* game, char* filename)/*{{{*/
{
  if (game == NULL)
    return -1;
  
  FILE* fp;
  if ((fp = fopen(filename, "r")) == NULL)
    return -2;

  //log1("Starting dict check!");
  int c;
  game->dictsize = 0;
  while((c = getc(fp)) != EOF)
    if (c == '\n')
      game->dictsize++;
  fseek(fp, 0, SEEK_SET);
  //log("Words: %d", game->dictsize);

  game->dict = malloc(game->dictsize * sizeof(char*));
  if (game->dict == NULL)
    return -3;

  int wordn = 0;
  for (wordn = 0; wordn < game->dictsize; wordn++)
  {
    char* word = mall_fword(fp);
    if (word == NULL)
      return -1; 

    game->dict[wordn] = word;
  }

  return 0;
}/*}}}*/

char* choose_random_word(game_t* game)/*{{{*/
{
  if (game == NULL)
    return NULL;

  int n = rand() % game->dictsize;
  return game->dict[n];
}/*}}}*/

int put_word_in_game(game_t* game)/*{{{*/
{
  if (game == NULL)
    return -1;

  int free_numbers[SCREEN_HEIGHT]; // The horizontal lines that don't yet have a
                                   // word in them.
  int free_numbers_len = 0; // The number of horizontal lines that ...

  for (int i = 0; i < SCREEN_HEIGHT; i++)
  {
    if (game->words[i].ptr == NULL)
      free_numbers[free_numbers_len++] = i;
  }

  if (free_numbers_len <= 0)
    return 0;

  int chosen_slot = free_numbers[rand() % free_numbers_len];
  char* word = choose_random_word(game);
  game->words[chosen_slot].ptr = word;
  game->words[chosen_slot].length = strlen(word);
  game->words[chosen_slot].clock = clock(); // This could be an issue if it overflows.
                                            // For a little bit the newest words could
                                            // look like the oldest for the program.
  
  // NOTE: We don't set the other values of the word, because when a spot is
  // emptied they should be all set to the init_game defaults.

  return 0;
}/*}}}*/

bool is_word_finished(scrword_t* word)/*{{{*/
{
  if (word == NULL)
    return true;

  return word->typedchars == word->length - 1;
}
/*}}}*/

void clear_word(game_t* game, int word_i)/*{{{*/
{
  game->words[word_i] = (scrword_t) {NULL, 0, 0, 0, 0, 0};
}/*}}}*/

// Criteria: oldest unfinished word.
int get_selected_word_i(game_t* game)/*{{{*/
{
  scrword_t *selected_word = NULL, *word;
  int selected_i = -1;
  
  for (int i = 0; i < WORD_AMOUNT; i++)
  {
    word = &game->words[i];

    // TODO: Maybe only select words whose characters are visible and non-typed,
    // because words start off the screen, and slowly appear.
    if (!(word->ptr == NULL) && !is_word_finished(word)) {
      if (selected_word == NULL || word->clock < selected_word->clock)
      {
        selected_word = word;
        selected_i = i;
      }
    }
  }

  return selected_i;
}/*}}}*/

double word_appearance_speed(game_t* game)/*{{{*/
{
  double secs_between_appearance = 500.0/(game->score+1);
  if (secs_between_appearance > 2)
    secs_between_appearance = 2;

  return secs_between_appearance;
}/*}}}*/

double word_move_speed(game_t* game)/*{{{*/
{
  double secs_between_moves = (100.0/(game->score+1));
  if (secs_between_moves > 0.5) // cutoff at around 190
    secs_between_moves = 0.5;

  return secs_between_moves;
}/*}}}*/

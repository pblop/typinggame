#include "util.h"

#include <stdlib.h>
#include <time.h>

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
    game->words[i] = (scrword_t) {NULL, 0, 0, 0};

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
  char* word = malloc(word_len * sizeof(char));
  if (word == NULL)
    return NULL;

  // Fill the word
  for (int i = 0; i < word_len; i++)
    word[i] = fgetc(fp);

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
  for (int wordn = 0; wordn < game->dictsize; wordn++)
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

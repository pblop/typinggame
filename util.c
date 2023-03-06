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

  srand(time(NULL));

  return 0;
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
  int wordsize = 0; // The size of the word, filled in the first iteration.
  int wordindex = 0;// The current index of the character we're copying inside
                    // the word, second iteration.
  int is_looking_for_size = 1;
  // This loop iterates every word twice. The first time we find out the word
  // size, and the second time we copy it onto its place on the array.
  while (wordn < game->dictsize)
  {
    c = getc(fp);
    //log("%c (%03d) - islook: %01d - ws: %02d - wi: %02d - wn: %05d", c, c, is_looking_for_size, wordsize, wordindex, wordn);
    if (is_looking_for_size)
    {
      if (c == '\n' || c == EOF)
      {
        // -1 because we also have to go back the \n / EOF
        if (fseek(fp, -wordsize-1, SEEK_CUR) != 0)
          return -3; // NOTE: We're not cleaning up after ourselves.
                     //       but if this function fails we're exiting the
                     //       game so it doesn't really matter.
        game->dict[wordn] = malloc(wordsize * sizeof(char));
        if (game->dict[wordn] == NULL)
          return -4;
        is_looking_for_size = 0;
      }
      else
        wordsize++;
    }
    else
    {
      game->dict[wordn][wordindex++] = c;

      if (c == '\n' || c == EOF)
      {
        wordn++;
        is_looking_for_size = 1;
        wordsize = 0;
        wordindex = 0;

        if (c == EOF)
          break; // We're done!
      }
    }
  }

  return 0;
}/*}}}*/

char* choose_random_word(game_t* game)
{
  if (game == NULL)
    return NULL;

  
  

  return 0;
}

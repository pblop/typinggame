#include "util.h"

int init_game(game_t* game)
{
  if (game == NULL)
    return -1;

  game->score = 0;
  game->lives = 3; // The initial number of lives.
  game->dictsize = -1;
  game->dict = NULL;

  return 0;
}

int load_dict(game_t* game, char* filename)
{
  int c;

  if (game == NULL)
    return -1;
  
  if ((game->dict = fopen(filename, "r")) == NULL)
    return -2;

  game->dictsize = 0;
  while ((c = getc(game->dict)) != EOF)
    if (c == '\n')
      game->dictsize++;

  return 0;
}

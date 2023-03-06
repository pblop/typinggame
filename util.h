#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>

typedef struct {
  // Goes up by the number of letters of each word you finish.
  int score;
  // Whenever you reach 0, you die!
  int lives;

  // Dictionaries must be a list of words, newline separated.
  // Number of words in the dictionary.
  int dictsize;
  // The dictionary.
  char** dict;
} game_t;

int init_game(game_t* game);
int load_dict(game_t* game, char* filename);
char* choose_random_word(game_t* game);

#endif

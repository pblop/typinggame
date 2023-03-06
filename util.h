#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include "common.h"

typedef struct {
  // A pointer to the word. If this word is not currently on screen, this
  // ptr is NULL.
  char* ptr;
  // The number of chars the user has correctly typed in the current word.
  int typedchars;

  // The distance of the first character from the right side of the screen.
  // When it reaches -1, you're down a life.
  int x;

  // The distance of the position where the word should be (its index in the
  // words array), to where it currently is.
  // This is to animate words falling off the screen whenever the user types
  // all their characters correctly.
  int y;
} scrword_t;

typedef struct {
  // Goes up by the number of letters of each word you finish.
  int score;
  // Whenever you reach 0, you die!
  int lives;

  // Dictionaries must be a list of words, newline separated.
  // Number of words in the dictionary.
  int dictsize;
  // The dictionary, an array of strings.
  char** dict;

  // Words currently in screen.
  scrword_t words[WORD_AMOUNT];

  // The current frame
  unsigned int frame;
} game_t;

int init_game(game_t* game);

char* mall_fword(FILE *fp); // used by load_dict
int load_dict(game_t* game, char* filename);
char* choose_random_word(game_t* game);

// This places a random word into the game.
int put_word_in_game(game_t* game);
bool is_word_finished(scrword_t* word);

#endif

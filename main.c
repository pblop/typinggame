#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "termio.h"
#include "util.h"

#define exitf(num) {unsetup_screen(); exit(num);}

void sigint_handler(int signum);
void setup_sigint_handler(void);

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

int main()
{

  /* Screen and exit setup */
  setup_screen();
  setup_sigint_handler();
  printf(HIDE_CURSOR CLEAR GOTO_HOME);

  /* Game stuff */

  /* Finish */
  unsetup_screen();

  return 0;
}

#include <stdio.h>
#include <signal.h>
#include "termio.h"

void sigint_handler(int signum);

void sigint_handler(int signum)
{
  unsetup_screen();
}

int main()
{
  sigset_t empty_set;
  struct sigaction action;

  /* Screen and exit setup */
  setup_screen();

  // Capture SIGINT and call unsetup_screen if program exited that way.
  sigemptyset(&empty_set);
  action.sa_handler = sigint_handler;
  action.sa_mask = empty_set;
  action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &action, NULL);

  printf(HIDE_CURSOR CLEAR GOTO_HOME);

  /* Game stuff */


  /* Finish */
  unsetup_screen();

  return 0;
}

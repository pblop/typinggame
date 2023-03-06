#include <stdio.h>

int main(void)
{
  while (1)
  {
    int c = getchar();

    printf("%03d - %03x - %c\n", c, c, c);
  }

  return 0;
}

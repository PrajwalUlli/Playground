#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#define ENTER 10
#define ESCAPE 27
#define BACKSPACE 127

int main(void)
{

  initscr();
  raw();
  noecho(); // dont echo pressed keys

  char ch;
  int cur_y, cur_x;

  bool QUIT = false;
  char *prompt = "[pcsh]$ ";

  char input[1024] = "";
  size_t cmdLen = 0;
  size_t cursorLine = 0;

  while (!QUIT)
  {
    /* reset screen with updated input buffer */
    move(cursorLine, 0);
    clrtoeol(); // clear till endofline

    mvprintw(cursorLine, 0, "%s", prompt); // mvprintw(y, x, string)
    getyx(stdscr, cur_y, cur_x);           // get cursor position after printing prompt
    mvprintw(cursorLine, cur_x, "%s", input);
    ch = getch();

    switch (ch)
    {
    case ESCAPE:
      QUIT = true;
      break;
    case ENTER:
      cursorLine++;
      memset(input, 0, strlen(input)); // clear input buffer
      cmdLen = 0;
      break;
    case BACKSPACE:
      if (cmdLen > 0)
      {
        cmdLen--;
        input[cmdLen] = '\0';
      }
      break;
    default:
      /* check for buffer overflow */
      if (cmdLen < sizeof(input))
      {
        input[cmdLen++] = ch;
      }
      break;
    }
  }

  refresh();
  endwin();
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

// #define BACKSPACE 127
// #define DOWN 258
// #define UP 259
#define ENTER 10
#define ESCAPE 27
#define CTRL_C 3
#define DATA_CAP 128
#define PROMPT "[pcsh]$ "

typedef struct
{
  char *data;
  size_t count;    // current index
  size_t capacity; // string cap
} DynamicArray;

/* Validate new capacity for DA_Append */
void DA_Space_Check(void *cond)
{
  if (!(cond)) // if passed cond is NULL then this runs
  {
    endwin();
    fprintf(stderr, "Out Of RAM!\n");
    exit(1);
  }
}

/* Dynamic String Array Resizing */
// as the user types the string is built with those chars using this func
void DA_Append(DynamicArray *DA, char item)
{
  if (DA->count >= DA->capacity)
  {
    DA->capacity = (DA->capacity == 0) ? DATA_CAP : DA->capacity * 2;
    void *new_space = calloc((DA->capacity) + 1, sizeof(char));
    DA_Space_Check(new_space);
    if (DA->data != NULL)
    {
      memcpy(new_space, DA->data, DA->count);
    }
    free(DA->data);
    DA->data = new_space;
  }
  DA->data[DA->count++] = item;
  DA->data[DA->count] = '\0';
}

int main(void)
{

  initscr();
  raw();    // i think it means dont handle any key presses
  noecho(); // dont echo pressed keys
  keypad(stdscr, TRUE);

  int ch; // could also be 'char ch', works this way for handling hardcoded special key values
  int cur_y, cur_x;

  bool QUIT = false;

  DynamicArray command = {0};

  DynamicArray command_history[32] = {0};
  size_t command_his_count = 0;
  // size_t cmdLen = 0;
  size_t cursorLine = 0;
  size_t upCount = 1;

  while (!QUIT)
  {
    // Helps with the backspace
    move(cursorLine, 0);
    clrtoeol(); // clear till endofline

    // mvprintw(y, x, string)
    mvprintw(cursorLine, 0, "%s", PROMPT);
    // get cursor position after printing prompt
    getyx(stdscr, cur_y, cur_x);
    // as at start the data is NULL, and the prompt will show (null)
    mvprintw(cursorLine, cur_x, "%s", (command.data == NULL ? "" : command.data));

    // capture keypress
    ch = getch();

    switch (ch)
    {
    case CTRL_C:
    case ESCAPE:
      QUIT = true;
      break;
    case ENTER:
      cursorLine++;
      mvprintw(cursorLine, 0, "%s is not recognized as an internal or external command", command.data);
      cursorLine++;
      command_history[command_his_count++] = command;
      command.data = NULL;
      command.count = 0;
      command.capacity = 0;
      // free(command.data);
      break;
    case KEY_BACKSPACE:
      if (command.count > 0)
      {
        command.data[--command.count] = '\0';
      }
      break;
    case KEY_UP: 
      if (command_his_count > 0 && upCount <= command_his_count)
      { 
        if (command.data == NULL)
          {
            command.data = calloc((strlen(command_history[command_his_count - upCount].data)+1), sizeof(char));
          }
        strcpy(command.data, command_history[command_his_count - upCount].data);
        upCount++;
      }
      break;
    case KEY_DOWN:
      if (command_his_count > 0 && upCount > 1)
      { 
        upCount--;
        if (command.data == NULL)
          {
            command.data = calloc((strlen(command_history[command_his_count - upCount].data)+1), sizeof(char));
          }
        strcpy(command.data, command_history[command_his_count - upCount].data);
      }
      break;
    default:
      DA_Append(&command, (char)ch);
      break;
    }
  }

  refresh();
  endwin();

  /* Free the calloc created space for the data block */
  for (int i = 0; i < (sizeof(command_history)/sizeof(command)); i++) {
    free(command_history[i].data);
  }
  free(command.data);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

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
} String_t;
typedef struct
{
  String *data;
  size_t count;    // current index tracker
  size_t capacity; // array cap
} String_Array_t;

#define DA_SPACE_CHECK(cond)            \
  do                                    \
  {                                     \
    if (!(cond))                        \
    {                                   \
      endwin();                         \
      fprintf(stderr, "Out Of RAM!\n"); \
      exit(EXIT_FAILURE);               \
    }                                   \
  } while (0)

// Macro to append a character to the DynamicArray.
// Also wrapped in do/while(0) to treat as a single block.
#define DA_APPEND(DA, item)                                          \
  do                                                                 \
  {                                                                  \
    if ((DA)->count >= (DA)->capacity)                               \
    {                                                                \
      (DA)->capacity = ((DA)->capacity == 0) ? DATA_CAP              \
                                             : (DA)->capacity * 2;   \
      void *_new_space = calloc(((DA)->capacity + 1), sizeof(char)); \
      DA_SPACE_CHECK(_new_space);                                    \
      if ((DA)->data != NULL)                                        \
      {                                                              \
        memcpy(_new_space, (DA)->data, (DA)->count);                 \
      }                                                              \
      free((DA)->data);                                              \
      (DA)->data = _new_space;                                       \
    }                                                                \
    (DA)->data[(DA)->count++] = (item);                              \
  } while (0)

int main(void)
{

  initscr();
  raw();                // i think it means dont handle any key presses
  noecho();             // dont echo pressed keys
  keypad(stdscr, TRUE); // enable arrow keys, function keys, etc

  int ch; // could also be 'char ch', works this way for handling hardcoded special key values
  int cur_y, cur_x;
  size_t cursorLine = 0; // size_t = unsigned int
  size_t max_hist_count = 0;

  // {0} valid only where you’re declaring a variable for the very first time
  String_t command = {0};
  String_Array_t command_history = {0};

  bool QUIT = false;
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
      DA_APPEND(&command_history, command);
      if (command_history.count > max_hist_count)
      {
        max_hist_count = command_history.count;
      }
      command = (String_t){0}; // create an anonymous “String” object (with all fields zero) and then assign it to “command.”
      break;
    case KEY_BACKSPACE:
      if (command.count > 0)
      {
        command.data[--command.count] = '\0';
      }
      break;
    case KEY_UP:
      if (command_history.count > 0)
      {
        command = (command_history.data)[--command_history.count];
      }
      break;
    case KEY_DOWN:
      if (command_history.count < max_hist_count)
      {
        command = (command_history.data)[++command_history.count];
      }
      break;
    default:
      DA_APPEND(&command, (char)ch);
      break;
    }
  }

  refresh();
  endwin();

  /* Free the calloc created space for the data block */
  for (int i = 0; i < command_history.count; i++)
  {
    // printf("%s\n", (command_history.data)[i].data);
    free((command_history.data)[i].data);
  }
  free(command_history.data);
  free(command.data);

  return 0;
}

// include headers
#include "loadfile.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macroes
#define NORMAL_MODE 0
#define COMMAND_MODE 1
#define INSERT_MODE 2

int edit(int mode, char ***text_buffer, int total_line);

// main function
int main(int argc, char *file[]) {

  int mode = NORMAL_MODE;

  // buffer to store the file content
  char **buffer = NULL;

  // initialise ncurses
  initscr();
  noecho();
  raw();
  // check if any parameter is entered
  // if true, read the path
  if (argc > 1) {
    // check if the file is a directory or a file
    // if it is a file, read the file
    int target_type = file_or_dir(file[1]);
    if (target_type == 3 || target_type == 1) {
      int lines = readfile(file[1], &buffer);
      // readfile loads the file content into buffer
      if (!(lines < 0)) {

        edit(mode, &buffer, lines);
        free_buffer(buffer, lines);
      } else {
        return -1;
      }

      // if it is a directory, print the directory
    } else if (target_type == 2) {
      printf("Target is a directory\n");
    }
  } else {
    // if no parameters entered, enter intro page(error for now)
    printf("No parameter entered\n");
  }

  // terminate ncurses
  endwin();
  return 0;
}

int edit(int mode, char ***text_buffer, int total_line) {
  int loop = 1;
  int max_x;
  int max_y;
  int ch;
  int row = 0;
  int col = 0;
  int top_line = 1;
  int left_col = 1;
  char MODE[8];
  switch (mode) {
  case 0:
    strcpy(MODE, "NORMAL");
    break;
  case 1:
    strcpy(MODE, "COMMAND");
    break;
  case 2:
    strcpy(MODE, "INSERT");
    break;
  default:
    printf("Unrecognised mode");
    return -1;
  }

  // loop for NORMAL mode
  while (loop) {
    clear();
    getmaxyx(stdscr, max_y, max_x);
    int display_lines = max_y - 1;
    int display_cols = max_x;

    for (int i = top_line - 1, line = 0; line < display_lines && i < total_line;
         i++, line++) {
      mvaddnstr(line, 0, (*text_buffer)[i] + left_col - 1, max_x);
    }

    mvprintw(max_y - 1, 0, MODE);
    move(row, col);

    refresh();
    ch = getch();

    if (ch == 'q') {
      loop = 0;
    } else if (ch == 'j') {
      int absolute_line = top_line - 1 + row;
      if (absolute_line < total_line - 1) {
        if (row < display_lines - 1) {
          row++;
        } else if (top_line + display_lines - 1 < total_line) {
          top_line++;
        }
      }
    } else if (ch == 'k') {
      int absolute_line = top_line - 1 + row;
      if (absolute_line > 0) {
        if (row > 0) {
          row--;
        } else if (top_line > 1) {
          top_line--;
        }
      }
    } else if (ch == 'l') {
      int current_line_index = top_line - 1 + row;
      if (current_line_index < total_line) {
        int line_length = strlen((*text_buffer)[current_line_index]);
        int absolute_col = left_col - 1 + col;
        if (absolute_col < line_length) {
          if (col < display_cols - 1) {
            col++;
          } else if (left_col + display_cols - 1 < line_length) {
            left_col++;
          }
        }
      }
    } else if (ch == 'h') {
      int current_line_index = top_line - 1 + row;
      if (current_line_index < total_line) {
        int absolute_col = left_col - 1 + col;
        if (absolute_col > 0) {
          if (col > 0) {
            col--;
          } else if (left_col > 1) {
            left_col--;
          }
        }
      }
    }
  }
  return 0;
}

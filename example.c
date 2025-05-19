#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Define the text buffer size
#define BUFFER_SIZE 1024

// Define modes
#define COMMAND_MODE 0
#define INSERT_MODE 1

// Global variables
char text_buffer[BUFFER_SIZE];
int cursor_x = 0, cursor_y = 0; // Cursor position
int mode = COMMAND_MODE;        // Start in command mode

void draw_status_bar() {
  move(LINES - 1, 0); // Move to the last line
  clrtoeol();         // Clear the line
  if (mode == COMMAND_MODE) {
    printw("-- COMMAND MODE --");
  } else if (mode == INSERT_MODE) {
    printw("-- INSERT MODE --");
  }
}

void refresh_screen() {
  clear();
  mvprintw(0, 0, "%s", text_buffer); // Print the text buffer
  move(cursor_y, cursor_x);          // Move the cursor
  draw_status_bar();
  refresh();
}

void handle_command_mode(int ch) {
  switch (ch) {
  case 'i': // Enter insert mode
    mode = INSERT_MODE;
    break;
  case 'h': // Move left
    if (cursor_x > 0)
      cursor_x--;
    break;
  case 'l': // Move right
    if (cursor_x < COLS - 1)
      cursor_x++;
    break;
  case 'j': // Move down
    if (cursor_y < LINES - 2)
      cursor_y++;
    break;
  case 'k': // Move up
    if (cursor_y > 0)
      cursor_y--;
    break;
  case 'q': // Quit
    endwin();
    exit(0);
    break;
  default:
    break;
  }
}

void handle_insert_mode(int ch) {
  if (ch == 27) { // ESC key to return to command mode
    mode = COMMAND_MODE;
  } else {
    // Insert character into the buffer
    int index = cursor_y * COLS + cursor_x;
    if (index < BUFFER_SIZE - 1) {
      text_buffer[index] = ch;
      cursor_x++; // Move cursor to the right
      if (cursor_x >= COLS) {
        cursor_x = 0;
        cursor_y++;
      }
    }
  }
}

int main() {
  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // Initialize text buffer
  memset(text_buffer, ' ', BUFFER_SIZE - 1);
  text_buffer[BUFFER_SIZE - 1] = '\0';

  // Main loop
  while (1) {
    refresh_screen();
    int ch = getch(); // Get user input
    if (mode == COMMAND_MODE) {
      handle_command_mode(ch);
    } else if (mode == INSERT_MODE) {
      handle_insert_mode(ch);
    }
  }

  // End ncurses
  endwin();
  return 0;
}

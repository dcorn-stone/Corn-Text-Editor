#include "loadfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 256
#define PRESET_LINES 512

int readfile(const char *filename, char ***out_buffer) {
  FILE *textptr = NULL;

  // allocate memory for the buffer
  // a buffer which each element is a pointer to a line a text
  unsigned int line_limit = PRESET_LINES;
  char **text_buffer = malloc(line_limit * sizeof(char *));

  // Handle allocation error
  if (!text_buffer) {
    printf("Memory allocation failed\n");
    return -1;
  }
  // Counter to track successfully allocated lines
  unsigned int lines_allocated = 0;

  // allocate memory for each element in the array
  for (unsigned int i = 0; i < line_limit; i++) {
    text_buffer[i] = malloc(LINE_SIZE * sizeof(char));
    if (!text_buffer[i]) {
      printf("Memory allocation for line %d failed\n", i);
      free_buffer(text_buffer, lines_allocated);
      return -1;
    }
    lines_allocated++;
  }

  // Open the file
  textptr = fopen(filename, "r");

  // If the file does not exist, create an empty file
  if (textptr == NULL) {
    textptr = fopen(filename, "w");
    if (textptr) {
      fclose(textptr);
    }

    textptr = fopen(filename, "r");

    // If it still fails, free memory and exit
    if (textptr == NULL) {
      free_buffer(text_buffer, lines_allocated);
      printf("Failed to open file\n");
      return -3;
    }
  }
  // fetch each line of the file into the pointer array
  unsigned int i = 0;
  while (1) {
    // if number of lines allocated are all used, allocate new chunks for it
    if (i == line_limit - 1) {
      unsigned int line_temp = line_limit;
      line_limit += PRESET_LINES;
      char **addline_temp = realloc(text_buffer, line_limit * sizeof(char *));
      if (!addline_temp) {
        printf("Memory reallocation for buffer failed");
        free_buffer(text_buffer, lines_allocated);
        fclose(textptr);
        return -2;
      }
      text_buffer = addline_temp;

      // initialise new lines allocated to the buffer
      for (unsigned int j = line_temp; j < line_limit; j++) {
        text_buffer[j] = malloc(LINE_SIZE * sizeof(char));
        if (!text_buffer[j]) {
          printf("Memory allocation for line %d failed\n", j);
          free_buffer(text_buffer, lines_allocated);
          fclose(textptr);
          return -1;
        }
        lines_allocated++;
      }
    }

    // if reached the end of the file, exit the loop
    if (!fgets(text_buffer[i], LINE_SIZE, textptr))
      break;

    // if the current line reading from the file is longer than the
    // memory allocated for this line, resize this line for more memory
    size_t current_size = LINE_SIZE;
    while (!strchr(text_buffer[i], '\n') && !feof(textptr)) {
      current_size += LINE_SIZE;
      char *temp = realloc(text_buffer[i], current_size);
      if (!temp) {
        printf("Memory reallocation for line %d failed\n", i);
        free_buffer(text_buffer, lines_allocated);
        fclose(textptr);
        return -2;
      }
      text_buffer[i] = temp;
      fgets(&text_buffer[i][strlen(text_buffer[i])], LINE_SIZE, textptr);
    }

    i++;
  }

  // set the output buffer to the text_buffer
  // the pointer entered will contain the content of the file
  *out_buffer = text_buffer;
  fclose(textptr);
  return i;
}

// check if thE PATH is a file or directory
int file_or_dir(const char *path) {
  struct stat path_stat;

  errno = 0;
  if (stat(path, &path_stat) != 0) {
    if (errno == ENOENT) {

      // File or directory does not exist
      // return and let the readfile function create it
      return 1;
    } else {
      perror("stat");
      return -1;
    }
  }
  // S_ISDIR checks if the path is a directory
  if (S_ISDIR(path_stat.st_mode)) {
    return 2;
  }
  // S_ISREG checks if the path is a regular file
  else if (S_ISREG(path_stat.st_mode)) {
    return 3;
  } else {
    return 4;
  }
}

// function used to free every line in the buffer and the buffer itself
void free_buffer(char **text_buffer, unsigned int allocated_lines) {
  for (unsigned int j = 0; j < allocated_lines; j++) {
    free(text_buffer[j]);
  }
  free(text_buffer);
}

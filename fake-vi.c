// include headers
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

// macroes
#define CHUNK_SIZE 1024
#define COMMAND_MODE 0
#define INSERT_MODE 1

// function declaration
int readfile(const char *filename, char **out_buffer);
int file_or_dir(const char *path);

// main function
int main(int argc, char *file[]) {
    
  //buffer to store the file content
  char *buffer = NULL;

  // initialise ncurses
  initscr();
  // check if any parameter is entered
  // if true, read the path
  if (argc > 1) {

    // check if the file is a directory or a file
    // if it is a file, read the file
    int target_type = file_or_dir(file[1]);
    if (target_type == 3 || target_type == 1){

        // readfile loads the file content into buffer
        if (readfile(file[1], &buffer) == 0){

            // print the file content
            printw("%s", buffer);

        } else {
            return -1;
        }

    // if it is a directory, print the directory
    } else if (target_type == 2){
      printw("Target is a directory");
    }

  // if no parameters entered, enter intro page(error for now)
  } else {
    printw("No parameter entered\n");
  }

  // display in ncurses
  refresh();

  // wait for any key to be pressed
  getch();

  // terminate ncurses
  endwin();
  free(buffer);
  return 0;
}


int readfile(const char *filename, char **out_buffer) {
    FILE *textptr = NULL;

    // Allocate an initial buffer of CHUNK_SIZE bytes
    size_t total_alloc = CHUNK_SIZE;
    char *text_buffer = malloc(total_alloc);

    // Handle allocation error
    if (!text_buffer) {
        printf("Memory allocation failed\n");
        return -1;
    }

    // Initialize used length to zero
    size_t used = 0;
    
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
            free(text_buffer);
            printf("Failed to open file\n");
            return -2;
        }
    }

    // Temporary buffer to read a line (or chunk) from the file
    char line[CHUNK_SIZE];
    while (fgets(line, CHUNK_SIZE, textptr)) {
        size_t line_len = strlen(line);

        // Check if the total needed size (current data + new line + terminating null) exceeds the
        // allocated space
        if (used + line_len + 1 > total_alloc) {
            total_alloc += CHUNK_SIZE;  // Increase by CHUNK_SIZE bytes

            // temporary pointer to reallocate memory
            char *tmp = realloc(text_buffer, total_alloc);
            if (tmp == NULL) {

                // On failure, free already allocated memory and close the file
                printf("Memory reallocation failed\n");
                free(text_buffer);
                fclose(textptr);
                return -3;
            }
            text_buffer = tmp;
        }
        // Append the new line into text_buffer at the position 'used'
        strcpy(text_buffer + used, line);
        used += line_len;
    }

    // Ensure the final string is null-terminated
    text_buffer[used] = '\0';

    // set the output buffer to the text_buffer
    // the pointer entered will contain the content of the file
    *out_buffer = text_buffer;
    fclose(textptr);
    return 0;
}

// check if the path is a file or directory
int file_or_dir(const char *path){
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
    }
    else {
        return 4;
    }

}
// #include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define COMMAND_MODE 0
#define INSERT_MODE 1

void readfile(char *filename);
int file_or_dir(const char *path);

int main(int param, char *file[]) {
  if (param > 1) {
    if (file_or_dir(file[1]) == 3 || file_or_dir(file[1]) == 1){
    readfile(file[1]);
    } else if (file_or_dir(file[1]) == 2){
      printf("Target is a directory");
    }
  } else {
    printf("No parameter entered\n");
  }
  return 0;
}

void readfile(char *filename) {
  FILE *textptr = NULL;
  char *text_buffer = malloc(BUFFER_SIZE);
  textptr = fopen(filename, "r");

  if (textptr == NULL) {
    textptr = fopen(filename, "w");
    fclose(textptr);
    textptr = fopen(filename, "r");
  }

  while (fgets(text_buffer, BUFFER_SIZE, textptr)) {
    printf("%s", text_buffer);
  }
  
  fclose(textptr);
  free(text_buffer);
  text_buffer = NULL;
}


int file_or_dir(const char *path){
    struct stat path_stat;

    errno = 0;
    if (stat(path, &path_stat) != 0) {
        if (errno == ENOENT) {
            return 1;
        } else {
            perror("stat");
            return -1;
        }
    }

    if (S_ISDIR(path_stat.st_mode)) {
        return 2;
    }

    else if (S_ISREG(path_stat.st_mode)) {
        return 3;
    }
    else {
        return 4;
    }

}
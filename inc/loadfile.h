#ifndef LOADFILE_H
#define LOADFILE_H

#include <errno.h>
#include <sys/stat.h>

// Read the content of a file into a buffer
// On success, returns 0 and sets *out_buffer to the content
// On failure, returns a negative value
int readfile(const char *filename, char ***out_buffer);

// Check whether a given path is a file, directory, or doesn't exist
// Returns:
//   1 for non-existent (will be created later)
//   2 for a directory
//   3 for a regular file
//   4 for any other file type
// or a negative value in case of error
int file_or_dir(const char *path);

// frees the buffer that stores the text
// frees every line of the buffer including the buffer itself
void free_buffer(char **text_buffer, unsigned int allocated_lines);

#endif // !LOADFILE_H

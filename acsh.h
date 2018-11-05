#include "structs.h"

#ifndef PROJ4_ACSH_H
#define PROJ4_ACSH_H

/// Allocates and stores user input into a char array.
/// Returns number of args stored in array.
size_t parse_user_input(char *line, input_t *input);

/// Releases memory allocated to heap.
/// Returns 0 if OK, -1 if error occurred.
int free_input_mem(input_t *input);

/// Releases memory allocated to heap.
/// Returns 0 if OK, -1 if error occurred.
int free_path_mem(list_t *paths);

/// Prints currently set path names to stdout
void display_path(list_t paths);

/// Appends a new path (add) to path structure.
/// Returns 0 if OK, -1 if error occurred.
int append_path(char *add, list_t *paths);

/// Removes the specified path from path structure.
/// Returns 0 if OK, -1 if error occurred.
int remove_path(char *remove, list_t *paths);

#endif //PROJ4_ACSH_H

#include "structs.h"

#ifndef PROJ4_ACSH_H
#define PROJ4_ACSH_H

/// Allocates and stores user input into a char array.
/// Returns number of args stored in array.
size_t parse_user_input(char *line, input_t *input);

/// Releases memory allocated to heap.
/// Returns 0 if OK, -1 if error occurred.
int free_heap_mem(input_t *input);

#endif //PROJ4_ACSH_H

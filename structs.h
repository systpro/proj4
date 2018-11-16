#ifndef PROJ4_STRUCTS_H
#define PROJ4_STRUCTS_H

typedef struct{
    size_t args_c;
    char **args_v;
} input_t;

typedef struct node_t{
    struct node_t *next;
    char *path;
} node_t;

typedef struct{
    size_t count;
    node_t *head;
    node_t *tail;
} list_t;

#endif //PROJ4_STRUCTS_H

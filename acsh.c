#include <stdio.h>
#include <tkPort.h>

#include "acsh.h"
#include "structs.h"

size_t parse_user_input(char *line, input_t *input){
    /// parse what the user entered into an array called input
    size_t input_size = 0;
    char * user_args;
    user_args = strtok(line, " ");
    for(int i = 0; i < input->args_c ; ++i) {
        if( user_args == NULL){break;}
        if( strlen(user_args)) {
            char *temp = calloc(strlen(user_args) + 1, sizeof(char));
            if( temp != NULL){
                input->args_v[i] = temp;
                input_size++;
            }

            strncpy(input->args_v[i], user_args, strlen(user_args));
            if( (i+2) == input->args_c){
                user_args = strtok(NULL, "\n");
            } else {
                user_args = strtok(NULL, " ");
            }
        }
    }
    return input_size;
}
int free_heap_mem(input_t *input){
    size_t i = 0;
    size_t tmp = input->args_c;
    for(i = 0; i < input->args_c; i++){
        free(input->args_v[i]);
    }
    free(input->args_v);
    if( i == tmp){
        return 0;
    }
    return -1;
}


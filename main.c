#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "acsh.h"
#include "structs.h"

int main() {
    list_t paths;
    paths.count = 0;
    paths.head = NULL;
    paths.tail = NULL;

    while(1){
        input_t input;
        input.args_c = 0;
        char line[40];

        /// the prompt
        printf("$ \t");
        /// read system standard input into char *line
        fgets(line, 40, stdin);
        /// count arguments received from the user
        for(int i = 0; line[i]; i++) {
            if( line[i] == ' ' || line[i] == '\n'){
                input.args_c++;
            }
        }

        /// allocate array called input
        input.args_v = (char **) calloc(input.args_c + 1, sizeof(char *));

        /// place what the user entered into array called input
        parse_user_input(line, &input);

        /// check if user is performing internal commands

        ///////////
        /// quit //
        ///////////
        if( strncmp(input.args_v[0], "quit", 4) == 0 ){
            if(free_input_mem(&input) < 0 || free_path_mem(&paths) < 0){
                printf("mem error!");
            } else{
                break;
            }
        }
        /////////
        /// cd //
        /////////
        else if( strncmp(input.args_v[0], "cd", 2) == 0){
            if( input.args_c < 2){
                free_input_mem(&input);
                continue;
            }
            else if( chdir(input.args_v[1]) < 0){
                printf("chdir failed!\n");
            } else{
                free_input_mem(&input);
                continue;
            }
        }
        ///////////
        /// path //
        ///////////
        else if( strncmp(input.args_v[0], "path", 4) == 0){
            if( input.args_c == 1){
                display_path(paths);
                free_input_mem(&input);
                continue;
            }
            if( input.args_c == 3){
                if( strncmp(input.args_v[1], "+", 1) == 0){
                    append_path(input.args_v[2], &paths);
                }
                else if( strncmp(input.args_v[1], "-", 1) == 0){
                    remove_path(input.args_v[2], &paths);
                } else {
                    printf("usage: %s, not a valid command\n", input.args_v[1]);
                }
            } else if ( input.args_c == 2) {
                printf("usage: %s, try 'path [+][-] /foo/bar'\n", input.args_v[1]);
            }
            else{
                printf("usage: %s, must specify a pathname\n", input.args_v[1]);
            }

        }
        //////////////
        /// execute //
        //////////////
        else if(strlen(input.args_v[0])){
            if(paths.count != 0) {
                if (execute(input, paths) == 0) {
                    ;
                }else{
                    printf("usage: %s, something's not right try again\n", input.args_v[0]);
                }
            } else{
                printf("usage: %s, must specify at least one path using path command\n", input.args_v[0]);
            }
        }
        free_input_mem(&input);
    }
    return 0;
}

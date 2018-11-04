#include <stdio.h>
#include <tkPort.h>

#include "acsh.h"
#include "structs.h"

int main() {
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
        input.args_v = (char **) calloc(input.args_c, sizeof(char *));

        /// place what the user entered into array called input
        parse_user_input(line, &input);

        /// check if user is performing internal commands
        ///////////
        /// quit //
        ///////////
        if( strncmp(input.args_v[0], "quit", 4) == 0 ){
            if( free_heap_mem(&input) < 0){
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
                free_heap_mem(&input);
                continue;
            }
            else if( chdir(input.args_v[1]) < 0){
                printf("chdir failed!\n");
            } else{
                /// start debug
                size_t size = 4096;
                char path[size];
                getcwd(path, size);
                printf("cwd = %s\n", path);
                /// end debug
                free_heap_mem(&input);
                continue;
            }
        }
        ///////////
        /// path //
        ///////////

    }
    return 0;
}
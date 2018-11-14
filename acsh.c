#include <stdio.h>
#include <tkPort.h>
#include <sys/wait.h>

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
            if(input->args_c == 1){
                if(user_args[strlen(user_args)-1]== '\n'){
                    user_args[strlen(user_args)-1] = '\0';
                }
                strncpy(input->args_v[i], user_args, strlen(user_args));
            }
            strncpy(input->args_v[i], user_args, strlen(user_args));
            if( (i+2) == input->args_c){
                user_args = strtok(NULL, "\n");
            } else {
                user_args = strtok(NULL, " ");
            }
        }
    }
    input->args_v[input_size]= (char *) 0;
    return input_size;
}

int free_input_mem(input_t *input){
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

int free_path_mem(list_t *paths){
    if( paths->count != 0){
        while( paths->head != NULL ){
            node_t *destroy = paths->head;
            paths->head = paths->head->next;
            free(destroy->path);
            free(destroy);
            paths->count--;
        }
        return 0;
    }
    return 0;
}

int free_path_arr(char **arr){
    int i = 0;
    for(; i < 2; i++){
        free(arr[i]);
    }
    if(i == 2){
        free(arr);
        return 0;
    } else{
        return -1;
    }
}

void display_path(list_t paths){
    if( paths.head == NULL) {
        printf("\n");
    }
    else if(paths.head->next == NULL){
        printf("%s\n", paths.head->path);
    } else{
        for( node_t *node = paths.head; node != NULL; node = node->next){
            if( node->next == NULL){
                printf("%s\n", node->path);
            } else{
                printf("%s:", node->path);
            }
        }
    }
}

int append_path(char *add, list_t *paths){
    if( paths->count > 0){
        node_t *node = paths->head;
        do{
            if( strcmp(node->path, add) == 0) {
                printf("path already contains: %s\n", add);
                return -1;
            }
            node = node->next;
        } while(node != NULL);
    }

    if( paths->count == 0){
        node_t *entry = calloc(1, sizeof(node_t));
        if( entry != NULL){
            char *size = calloc(strlen(add) + 1, sizeof(char));
            if( size != NULL){
                entry->path = size;
                strcpy(entry->path, add);
            }
            entry->next = NULL;
            paths->head = entry;
            paths->tail = entry;
            paths->count++;
        }
    } else {
        node_t *entry = calloc(1, sizeof(node_t));
        if( entry != NULL){
            char *size = calloc(strlen(add) + 1, sizeof(char));
            if( size != NULL){
                entry->path = size;
                strcpy(entry->path, add);
                entry->next = NULL;
                for(node_t *node = paths->head; node != NULL; node = node->next){
                    if( node->next == NULL){
                        node->next = entry;
                        break;
                    }
                }
                paths->tail = entry;
                paths->count++;
            }
        }
    }
    return 0;
}

int remove_path(char *remove, list_t *paths){
    if( paths->count == 1){
        if( strcmp(paths->head->path, remove) == 0){
            free(paths->head->path);
            free(paths->head);
            paths->head = NULL;
            paths->tail = NULL;
            paths->count--;
        } else {
            printf("path: %s not found\n", remove);
            return -1;
        }
    }
    if( paths->count > 1){
        if( strcmp(paths->head->path, remove) == 0){
            // remove first node
            node_t *destroy = paths->head;
            paths->head = paths->head->next;
            free(destroy->path);
            free(destroy);
            paths->count--;
        } else {
            for (node_t *node = paths->head; node != NULL; node = node->next) {
                if (strcmp(node->next->path, remove) == 0) {
                    // remove node after current node
                    node_t *destroy = node->next;
                    node->next = node->next->next;
                    free(destroy->path);
                    free(destroy);
                    break;
                }
            }
        }
    }
    return 0;
}
char *const *paths_to_array(list_t paths){
    char **path_array = calloc(2, sizeof(char*));
    path_array[0] = calloc(sizeof(paths.head->path) + 6, sizeof(char));
    strcpy(path_array[0], "PATH=");
    strcat(path_array[0], paths.head->path);
    int i = 1;
    for(node_t *node = paths.head->next; node != NULL; node = node->next){
        char *temp = realloc(path_array[0], sizeof(node->path));
        if(temp != NULL){
            path_array[0] = temp;
            strcat(path_array[0], node->path);
            i++;
        }
    }
    if(i == paths.count){
        path_array[1] = (char *) 0;
        return path_array;
    } else{ return  NULL;}
}


int execute(char *const *paths, input_t const input, list_t path_list){
    int pid, status;
    pid = fork();
    switch(pid){
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            // child process
            // try each path in path_list to form command.
            // existing bugs:
            // does not load env. vars (e.g. echo $HOME), is this an issue?
            // function logic follows the book example very closely, is this an issue?

            for(node_t *node = path_list.head; node != NULL; node = node->next) {
                size_t str_size = strlen(node->path) + strlen(input.args_v[0]) + 2;
                char *command = calloc(str_size, sizeof(char));
                strcpy(command, node->path);
                strcat(command, "/");
                strcat(command, input.args_v[0]);
                execve(command, input.args_v, paths);
            }
            perror("execve failed");
            exit(1);
        default:
            while (wait(&status) != pid){
                return 0;
            }
    }
}

// gcc compute_sequence_3.c -o compute_sequence_3 && ./compute_sequence_3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define READ 0
#define WRITE 1

int main(){
    pid_t pid;
    int n = 100;
    char input[BUFFER_SIZE];
    char lhs[BUFFER_SIZE], rhs[BUFFER_SIZE];

    while(1){
        fgets(input, BUFFER_SIZE, stdin);
        if(strcmp(input, "quit\n") == 0){
            break;
        }
        sscanf(input, "%s | %s", lhs, rhs);
        if(strcmp(lhs, "./compute_sequence_1") || strcmp(rhs, "./compute_sequence_2")){
            printf("Invalid command\n\n");
            continue;
        }

        pid = fork();
        if(pid < 0){
            perror("Fork 1 failed");
            return 1;
        }
        if(pid == 0){
            int fd[2];
            pipe(fd);
            pid = fork();
            if(pid < 0){
                perror("Fork 2 failed");
                return 1;
            }
            if(pid > 0){
                close(fd[READ]);
                if(dup2(fd[WRITE], STDOUT_FILENO) == -1){
                    perror("dup2 parent failed");
                }
                close(fd[WRITE]);
                execlp(lhs, lhs, NULL);
            }
            else{
                close(fd[WRITE]);
                if(dup2(fd[READ], STDIN_FILENO) == -1){
                    perror("dup2 child failed");
                }
                close(fd[READ]);
                execlp(rhs, rhs, NULL);
            }
        }
    }
    
    return 0;
}
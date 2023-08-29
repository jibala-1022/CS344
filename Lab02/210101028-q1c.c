#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid;

    pid = fork();
    if(pid < 0){
        printf("Fork 1 failed\n");
        exit(1);
    }
    else if(pid == 0){
        execle("210101028-q1a", "210101028-q1a", NULL, NULL);
    }

    pid = fork();
    if(pid < 0){
        printf("Fork 2 failed\n");
        exit(1);
    }
    else if(pid == 0){
        execl("210101028-q1a", "210101028-q1a", "5", NULL);
    }

    pid = fork();
    if(pid < 0){
        printf("Fork 3 failed\n");
        exit(1);
    }
    else if(pid == 0){
        char* args[] = {"210101028-q1a", "10", "20", "30", "40", "50", "60", "70", "80", "90", "99", NULL};
        execve("210101028-q1a", args, NULL);
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid;

    char* n;

    pid = fork();
    if(pid < 0){
        printf("Fork 1 failed\n");
        exit(1);
    }
    else if(pid == 0){
        unsetenv("n");
        execle("210101028-q1b", "210101028-q1b", NULL, NULL);
    }

    pid = fork();
    if(pid < 0){
        printf("Fork 2 failed\n");
        exit(1);
    }
    else if(pid == 0){
        setenv("n", "5", 0);
        execl("210101028-q1b", "210101028-q1b", NULL);
    }

    pid = fork();
    if(pid < 0){
        printf("Fork 3 failed\n");
        exit(1);
    }
    else if(pid == 0){
        char* args[] = {"210101028-q1b", NULL};
        char* envp[] = {"n=10 20 30 40 50 60 70 80 90 99"};
        execve("210101028-q1b", args, envp);
    }

    return 0;
}
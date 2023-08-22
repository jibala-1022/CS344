// gcc qb.c && ./a.out <number>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Enter N as command line argument\n");
        return 1;
    }

    FILE* f = fopen("fb.txt", "a");

    int n = atoi(argv[1]);

    pid_t pid = fork();
    
    if(pid < 0){
        printf("Fork failed\n");
        exit(1);
    }
    else if(pid == 0){
        fprintf(f, "Child <%d>: ", getpid());
        while(n != 1){
            fprintf(f, "%d ", n);
            n = (n%2) ? 3*n+1 : n/2;
        }
        fprintf(f, "%d\n", n);

        fprintf(f, "Child File Position Indicator: %ld\n\n", ftell(f));
    }
    else{
        wait(NULL);
        fprintf(f, "Parent File Position Indicator: %ld\n\n", ftell(f));
    }

    fclose(f);

    return 0;
}
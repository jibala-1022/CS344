// gcc qc.c && ./a.out <number>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define CHILDREN_COUNT 50

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Enter N as command line argument\n");
        return 1;
    }

    FILE* f = fopen("fc.txt", "a");

    int n = atoi(argv[1]);

    for(int i=1; i<=CHILDREN_COUNT; i++){
        pid_t pid = fork();
        
        if(pid < 0){
            printf("Fork failed\n");
            exit(1);
        }
        else if(pid == 0){
            n *= i;
            fprintf(f, "Child %.2d <%d>: ", i, getpid());
            while(n != 1){
                fprintf(f, "%d ", n);
                n = (n%2) ? 3*n+1 : n/2;
            }
            fprintf(f, "%d\n", n);

            fprintf(f, "Child %.2d File Position Indicator: %ld\n\n", i, ftell(f));

            exit(0);
        }
    }

    int status;
    while(wait(&status) != -1);
    fprintf(f, "Parent File Position Indicator: %ld\n\n", ftell(f)); 

    fclose(f);

    return 0;
}
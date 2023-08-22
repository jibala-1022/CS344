// gcc qa.c && ./a.out <number>

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
    int n = atoi(argv[1]);

    pid_t pid = fork();
    
    if(pid < 0){
        printf("Fork failed\n");
        exit(1);
    }
    else if(pid == 0){
        while(n != 1){
            printf("%d ", n);
            n = (n%2) ? 3*n+1 : n/2;
        }
        printf("%d ", n);
    }
    else{
        wait(NULL);
        printf("\n");
    }

    return 0;
}
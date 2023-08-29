#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int sequence(int n){
    while(n != 1){
        printf("%d ", n);
        if(n%2) n = 3*n+1;
        else n = n/2;
    }
    printf("1\n");
}

int main(int argc, char* argv[]){
    int n;
    if(argc==1){
        n = 100;
        sequence(n);
    }
    else{
        for(int i=1; i<argc; i++){
            n = atoi(argv[i]);
            sequence(n);
        }
    }
    return 0;
}
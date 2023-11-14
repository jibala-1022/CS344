// gcc compute_sequence_2.c -o compute_sequence_2 && ./compute_sequence_2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int sequence(int n){
    while(n != 1){
        if(n%2) n = 3*n+1;
        else n = n/2;
        printf("%d ", n);
    }
    printf("\n");
}

int main(){
    int n = 100;
    while(scanf("%d", &n) == 1){
        sequence(n);
        if(n == 1){
            break;
        }
    }
    return 0;
}
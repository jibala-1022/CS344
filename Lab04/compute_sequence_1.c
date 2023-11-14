// gcc compute_sequence_1.c -o compute_sequence_1 && ./compute_sequence_1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    sequence(n);
    return 0;
}
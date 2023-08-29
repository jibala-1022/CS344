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

int main(){
    char* n = getenv("n");
    int num;
    if(n == NULL){
        num = 100;
        sequence(num);
    }
    else{
        int i=0;
        num = 0;
        while(n[i] != '\0'){
            if(n[i] == ' '){
                sequence(num);
                num = 0;
            }
            else{
                num = 10*num + (n[i]-'0');
            }
            i++;
        }
        sequence(num);
    }
    return 0;
}
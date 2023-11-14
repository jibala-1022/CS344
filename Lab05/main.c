#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

pthread_mutex_t lock;
// pthread_mutex_lock(&lock);

static int *client_count;

int main(){
    char* ball;
    char* batsman;
    char* bowler;
    char* result;
    char* data = "72.3 Patel to Raina, no run, comes well forward and defends";
    int offset=0;
        
        // sscanf(data + offset, "%s %s to %s, %s", ball, bowler, batsman, result);
        printf("%s %s to %s, %s\n", ball, bowler, batsman, result);
        // printf("%s", data + offset);
        offset += strlen(data + offset);
}
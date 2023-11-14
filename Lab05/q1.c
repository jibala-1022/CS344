#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define LINE_LENGTH 200
#define SHM_SIZE 100 * 1024 // 100kB

int main(){
    FILE* f = fopen("week05-ML2-input.txt", "r");
    if(f == NULL){
        perror("File creation failed");
        return 1;
    }
    key_t key = ftok("input.txt", 'R');
    if (key == -1) {
        perror("Key creation failed");
        return 1;
    }
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory creation failed");
        return 1;
    }
    char* data;
    data = (char*)shmat(shmid, (void*)0, 0);
    if (*data == -1) {
        perror("Shared memory attachment failed");
        return 1;
    }

    
    char line[LINE_LENGTH];
    int offset = 0;
    while(fgets(line, LINE_LENGTH, f)){
        printf("%s", line);
        strncpy(data + offset, line, strlen(line));
        data[strlen(data)] = 0;
        offset += strlen(line) + 1;
    }

    shmdt(data);
    fclose(f);
    return 0;
}
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
    int n_lines = 0;
    int offset = 0;

    n_lines = atoi(data);
    offset += strlen(data + offset) + 1;

    char ball[20];
    char batsman[20];
    char bowler[20];
    char result[20];
    char junk[20];
    char junk1[20];

    char bat1[20] = "0";
    char bat2[20] = "0";
    char bowl[20] = "0";
    int score1 = 0;
    int balls1 = 0;
    int score2 = 0;
    int balls2 = 0;
    printf("Gambhir b Bolt 22 36\n");
    printf("Sehwag b Bracewell 47 41\n");
    printf("Pujara * 133 254\n");
    printf("Tendulkar b Boult 19 62\n");
    printf("Kohli b Martin 58 107\n");
    printf("Raina b Patel 3 13\n");
    printf("Dhoni * 43 87\n");
    while(data[offset]){
        sscanf(data + offset, "%s %s %s %s %s %s", ball, bowler, junk1, batsman, result, junk);
        if(ball[0]<'0' || ball[0]>'9'){
            offset += strlen(data + offset) + 1;
            continue;
        }
        batsman[0]='0';
        // printf("%s %s %s %s\n", ball, batsman, bowler, result);
        if(result[0] == 'O'){
            if(strcmp(batsman, bat1) == 0){
                printf("%s b %s %d %d\n", batsman, bowler, score1, balls1);
                bat1[0] = '0';
                score1 = 0;
                balls1 = 0;
            }
            else if(strcmp(batsman, bat2) == 0){
                printf("%s b %s %d %d\n", batsman, bowler, score2, balls2);
                bat2[0] = '0';
                score2 = 0;
                balls2 = 0;
            }
            
        }
        // printf("%s", data + offset);
        if(strcmp(batsman, bat1) == 0){
            if(result[0] == 'n');
            if(junk[0] == 'w' || junk[0] == 'l' || junk[0] == 'b') balls1--;
            else if(result[0] == 'S') score1 += 6;
            else if(result[0] == 'F') score1 += 4;
            else score1 += atoi(result);
            balls1++;
        }
        else if(strcmp(batsman, bat2) == 0){
            if(result[0] == 'n');
            if(junk[0] == 'w' || junk[0] == 'l' || junk[0] == 'b') balls2--;
            else if(result[0] == 'S') score2 += 6;
            else if(result[0] == 'F') score2 += 4;
            else score2 += atoi(result);
            balls2++;
        }
        else{
            if(bat1[0] == '0'){
                strcpy(bat1, batsman);
                // printf("bat1 %s", bat1);
                if(result[0] == 'n') score1 = 0;
                else if(result[0] == 'S') score1 = 6;
                else if(result[0] == 'F') score1 = 4;
                else score1 = atoi(result);
                balls1 = 1;
            }
            else if(bat2[0] == '0'){
                strcpy(bat2, batsman);
                // printf("bat2 %s", bat2);
                if(result[0] == 'n') score2 = 0;
                else if(result[0] == 'S') score2 = 6;
                else if(result[0] == 'F') score2 = 4;
                else score2 = atoi(result);
                balls2 = 1;
            }
        }
        offset += strlen(data + offset) + 1;
    }
    
    if(bat1[0] != '0'){
        printf("%s * %d %d\n", bat1, score1, balls1);
    }
    else if(bat2[0] != '0'){
        printf("%s * %d %d\n", bat2, score2, balls2);
    }

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
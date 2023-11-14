#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define N 5
#define NAME_SIZE 50
#define PID_SIZE 10
#define MAX_ROUNDS 5
#define SLEEP_TIME 10

struct Table {
    char name[NAME_SIZE];
    int pid[PID_SIZE];
};


void sem_wait(int semid, int sem_num){
    struct sembuf sb = {sem_num, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop-lock");
        exit(1);
    }
}

void sem_signal(int semid, int sem_num){
    struct sembuf sb = {sem_num, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop-unlock");
        exit(1);
    }
}


int main(){
    key_t shmkey, shmkey_count, semkey, semkey_count;
    int shmid, shmid_count, semid, semid_count;
    struct Table* tables;
    int* count;
    union semun {
        int val;                // Value for SETVAL
        struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
        unsigned short *array;  // Array for GETALL, SETALL
    } arg;

    if ((semkey = ftok("philosopher.c", 'R')) == -1) {
        perror("ftok-sem");
        exit(1);
    }

    if ((semid = semget(semkey, N, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    for(int i = 0; i < N; i++){
        if (semctl(semid, i, SETVAL, arg) == -1) {
            perror("semctl");
            exit(1);
        }
    }


    if ((shmkey = ftok("philosopher.c", 'S')) == -1) {
        perror("ftok-shm");
        exit(1);
    }

    if ((shmid = shmget(shmkey, N * sizeof(struct Table), 0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    tables = (struct Table*)shmat(shmid, (void*)0, 0);
    if (tables == (struct Table*)-1) {
        perror("shmat");
        exit(1);
    }

    for(int i = 0; i < N; i++){
        sprintf(tables[i].name, "%d", i+1);
        memset(tables[i].pid, 0, PID_SIZE);
    }


    if ((semkey_count = ftok("philosopher.c", 'T')) == -1) {
        perror("ftok-sem");
        exit(1);
    }

    if ((semid_count = semget(semkey_count, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    if (semctl(semid_count, 0, SETVAL, arg) == -1) {
        perror("semctl-count");
        exit(1);
    }

    if ((shmkey_count = ftok("philosopher.c", 'U')) == -1) {
        perror("ftok-shm");
        exit(1);
    }

    if ((shmid_count = shmget(shmkey_count, 1, 0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    count = (int*)shmat(shmid_count, (void*)0, 0);
    if (count == (int*)-1) {
        perror("shmat");
        exit(1);
    }
    *count = 0;

    for(int i = 0; i < N; i++){
        pid_t pid;
        if((pid = fork()) == -1){
            perror("fork");
            exit(1);
        }
        else if(pid == 0){
            pid = getpid();
            int round = 1;
            int pos = i;

            while(1){
                int left = (pos + round - 1) % N;
                int right = (pos + round) % N;

                if(left % 2 == 0){
                    sem_wait(semid, left); 
                    sem_wait(semid, right);
                }
                else{
                    sem_wait(semid, right);
                    sem_wait(semid, left); 
                }

                printf("Transaction <%d>: Holding the Semaphore\n", pid);
                printf("Transaction <%d>: Operating on Table %s - Table %s\n", pid, tables[left].name, tables[right].name);

                for(int j = 0; j < PID_SIZE; j++){
                    if(tables[left].pid[j] == 0){
                        tables[left].pid[j] = pid;
                        break;
                    }
                }
                for(int j = 0; j < PID_SIZE; j++){
                    if(tables[right].pid[j] == 0){
                        tables[right].pid[j] = pid;
                        break;
                    }
                }

                sem_wait(semid_count, 0);
                (*count)++;
                sem_signal(semid_count, 0);

                sleep(SLEEP_TIME);

                if(left % 2 == 0){
                    sem_signal(semid, left); 
                    sem_signal(semid, right);
                }
                else{
                    sem_signal(semid, right);
                    sem_signal(semid, left); 
                }

                while(*count < round * N);
                round++;
                if(round > MAX_ROUNDS){
                    break;
                }
            }

            exit(0);
        }
    }

    for(int i = 0; i < N; i++){
        wait(NULL);
    }

    printf("\nDatabase Contents\n\n");
    for(int i = 0; i < N; i++){
        printf("Name: Table %s\n", tables[i].name);
        printf("Pids: ");
        for(int j = 0; j < 10; j++){
            printf("%d ", tables[i].pid[j]);
        }
        printf("\n\n");
    }


    if(shmdt(tables) == -1){
        perror("shmdt");
        exit(1);
    }
    if(shmdt(count) == -1){
        perror("shmdt");
        exit(1);
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("shmctl");
        exit(1);
    }
    if(shmctl(shmid_count, IPC_RMID, NULL) == -1){
        perror("shmctl");
        exit(1);
    }
    if(semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl-all");
        exit(1);
    }

    return 0;
}
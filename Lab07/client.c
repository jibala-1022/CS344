// gcc client.c -o client && ./client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SHM_SIZE 25
#define MAX_REQUESTS 100
#define N_CHILDREN 2
#define MUTEX 0
#define EMPTY 1
#define FULL 2

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
    key_t shm_key, sem_key;
    int shmid, semid;
    pid_t pid;
    int* data;
    union semun {
        int val;                // Value for SETVAL
        struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
        unsigned short *array;  // Array for GETALL, SETALL
        // struct seminfo *__buf;  // Buffer for IPC_INFO (Linux-specific)
    } arg;

    if ((shm_key = ftok("server.c", 'R')) == -1) {
        perror("ftok-shm");
        exit(1);
    }

    if ((shmid = shmget(shm_key, SHM_SIZE + 2, 0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((sem_key = ftok("client.c", 'R')) == -1) {
        perror("ftok-sem");
        exit(1);
    }

    if ((semid = semget(sem_key, 3, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    if (semctl(semid, MUTEX, SETVAL, arg) == -1) {
        perror("semctl-mutex");
        exit(1);
    }
    arg.val = SHM_SIZE;
    if (semctl(semid, EMPTY, SETVAL, arg) == -1) {
        perror("semctl-empty");
        exit(1);
    }
    arg.val = 0;
    if (semctl(semid, FULL, SETVAL, arg) == -1) {
        perror("semctl-full");
        exit(1);
    }

    data = (int*)shmat(shmid, (void*)0, 0);
    if (*data == -1) {
        perror("shmat");
        exit(1);
    }

    *(data + SHM_SIZE) = 0;
    *(data + SHM_SIZE + 1) = 0;

    for(int i=0; i < MAX_REQUESTS; i++){
        sem_wait(semid, EMPTY);
        sem_wait(semid, MUTEX);

        printf("Enqueue HTTP request %d\n", i+1);
        *(data + (i % SHM_SIZE)) = i+1;

        sem_signal(semid, MUTEX);
        sem_signal(semid, FULL);
    }

    sem_signal(semid, EMPTY);
    sem_signal(semid, FULL);

    sleep(1);
    semctl(semid, 0, IPC_RMID, arg);
    shmdt(data);
    return 0;
}
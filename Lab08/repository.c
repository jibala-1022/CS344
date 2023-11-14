#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SHM_SIZE 1024
#define RW_MUTEX 0
#define MUTEX 1

int main(){
    key_t shmkey, shmkey_count, semkey;
    int shmid, shmid_count, semid;
    char* data;
    int* read_count;
    union semun {
        int val;                // Value for SETVAL
        struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
        unsigned short *array;  // Array for GETALL, SETALL
    } arg;

    if ((shmkey = ftok("repository.c", 'R')) == -1) {
        perror("ftok-shm");
        exit(1);
    }

    if ((shmid = shmget(shmkey, SHM_SIZE, 0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    data = (char*)shmat(shmid, (void*)0, 0);
    if (*data == -1) {
        perror("shmat");
        exit(1);
    }

    if ((shmkey_count = ftok("writer.c", 'R')) == -1) {
        perror("ftok-rc");
        exit(1);
    }

    if ((shmid_count = shmget(shmkey_count, 1, 0666 | IPC_CREAT)) == -1) {
        perror("shmget-rc");
        exit(1);
    }

    read_count = (int*)shmat(shmid_count, (void*)0, 0);
    if (*read_count == -1) {
        perror("shmat-rc");
        exit(1);
    }

    if ((semkey = ftok("reader.c", 'R')) == -1) {
        perror("ftok-sem");
        exit(1);
    }

    if ((semid = semget(semkey, 2, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    if (semctl(semid, RW_MUTEX, SETVAL, arg) == -1) {
        perror("semctl-rw-mutex");
        exit(1);
    }
    if (semctl(semid, MUTEX, SETVAL, arg) == -1) {
        perror("semctl-mutex");
        exit(1);
    }

    *read_count = 0;

    strcpy(data, "#include <stdio.h>\nint main(){\n    printf(\"Hello world!\\n\");\n}\n\0");

    printf("Sleeping...\nPress Enter to exit\n");
    scanf("%*c");


    if(shmdt(data) == -1){
        perror("shmdt");
        exit(1);
    }
    if(shmdt(read_count) == -1){
        perror("shmdt-rc");
        exit(1);
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("shmctl");
        exit(1);
    }
    if(shmctl(shmid_count, IPC_RMID, NULL) == -1){
        perror("shmctl-rc");
        exit(1);
    }
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl-all");
        exit(1);
    }
    return 0;
}
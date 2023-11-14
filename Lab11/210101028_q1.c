// gcc -lpthread 210101028_q1.c -o q1 && ./q1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;
FILE* fout;
int result[400][400] = {0};

typedef struct {
    int tid;
    int subA[400][100];
    int subB[100][400];
} targs;

void print_matrix(int matrix[400][400]){
    
}

void fprint_matrix(int matrix[400][400]){
    
}


void* matmul(void* vargs){
    targs* args = (targs*)vargs;
    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            int sum = 0;
            for(int k=0; k<100; k++){
                sum += args->subA[i][k] * args->subB[k][j];
            }
            pthread_mutex_lock(&mutex);
            result[i][j] += sum;
            // fprintf(fout, "Thread %d: result[%d][%d] = %d\n", args->tid, i, j, result[i][j]);
            pthread_mutex_unlock(&mutex);
        }
    }
    free(args);
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL)); // sets the rng seed
    fout = fopen("result.txt", "w");

    int A[400][400], B[400][400];

    // random distribution between 0 to 10 inclusive
    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            A[i][j] = rand() % 11;
            B[i][j] = rand() % 11;
        }
    }


    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_t tids[4];

    for(int t=0; t<4; t++){
        targs* args = (targs*)malloc(sizeof(targs));
        args->tid = t;
        for(int i=0; i<400; i++){
            for(int j=0; j<100; j++){
                args->subA[i][j] = A[i][100*t + j];
            }
        }
        for(int i=0; i<100; i++){
            for(int j=0; j<400; j++){
                args->subB[i][j] = B[100*t + i][j];
            }
        }
        pthread_create(tids + t, NULL, matmul, (void*)args);
    }

    for(int i=0; i<4; i++){
        pthread_join(tids[i], NULL);
    }


    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            fprintf(fout, "%d ", result[i][j]);
        }
        fprintf(fout, "\n");
    }

    return 0;
}
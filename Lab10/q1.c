// gcc -lpthread q1.c -o q1 && ./q1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 11

typedef struct {
    int row;
    int col;
} parameters;

typedef struct {
    parameters params;
    int res_ind;
} targs;

int sudoku[9][9];
int result[N_THREADS];

void *row_check(void* vargs) {
    targs* args = (targs*)vargs;
    int res_ind = args->res_ind;
    for(int i=0; i<9; i++){
        int vis[10] = {0};
        for(int j=0; j<9; j++){
            int cell = sudoku[i][j];
            if(cell<1 || cell>9 || vis[cell]){
                result[res_ind] = 0;
                free(args);
                pthread_exit(NULL);
            }
            else{
                vis[cell] = 1;
            }
        }
    }
    result[res_ind] = 1;
    free(args);
    pthread_exit(NULL);
}

void *col_check(void* vargs) {
    targs* args = (targs*)vargs;
    int res_ind = args->res_ind;
    for(int j=0; j<9; j++){
        int vis[10] = {0};
        for(int i=0; i<9; i++){
            int cell = sudoku[i][j];
            if(cell<1 || cell>9 || vis[cell]){
                result[res_ind] = 0;
                free(args);
                pthread_exit(NULL);
            }
            else{
                vis[cell] = 1;
            }
        }
    }
    result[res_ind] = 1;
    free(args);
    pthread_exit(NULL);
}

void *grid_check(void* vargs) {
    targs* args = (targs*)vargs;
    int row = args->params.row;
    int col = args->params.col;
    int res_ind = args->res_ind;
    int vis[10] = {0};
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            int cell = sudoku[row+i][col+j];
            if(cell<1 || cell>9 || vis[cell]){
                result[res_ind] = 0;
                free(args);
                pthread_exit(NULL);
            }
            else{
                vis[cell] = 1;
            }
        }
    }
    result[res_ind] = 1;
    free(args);
    pthread_exit(NULL);
}

int main() {
    FILE* f = fopen("week10-ML2-input1.txt", "r");
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            fscanf(f, "%d", &sudoku[i][j]);
        }
    }

    pthread_t threads[N_THREADS];
    targs* args;
    int res_ind = 0;

    args = (targs*)malloc(sizeof(targs));
    args->params.row = -1;
    args->params.col = -1;
    args->res_ind = res_ind;
    pthread_create(&threads[res_ind++], NULL, col_check, (void*)args);
    
    args = (targs*)malloc(sizeof(targs));
    args->params.row = -1;
    args->params.col = -1;
    args->res_ind = res_ind;
    pthread_create(&threads[res_ind++], NULL, row_check, (void*)args);
    
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            args = (targs*)malloc(sizeof(targs));
            args->params.row = 3*i;
            args->params.col = 3*j;
            args->res_ind = res_ind;
            pthread_create(&threads[res_ind++], NULL, grid_check, (void*)args);
        }
    }

    for(int i=0; i<N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    int valid = 1;

    for(int i=0; i<N_THREADS; i++){
        if(result[i]==0){
            valid=0;
            break;
        }
    }

    if(valid) printf("Valid solution\n");
    else printf("Invalid solution\n");

    return 0;
}
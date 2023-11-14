#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_CHILDREN 8

struct msgbuf {
	long mtype;
	int row;
	int col;
};
int nx, mx, ny, my, ni, mi;

void computeEdgeValue(int row, int col, int Kx[nx][mx], int Ky[ny][my], int I[ni][mi], int nx, int mx, int ny, int my, int ni, int mi){
	FILE* fout;
	double gx=0;
	double gy=0;
	double G;

	if((fout = fopen("ML2-output.txt", "a")) == NULL){
		perror("fopen-out");
		exit(1);
	}

	for(int i=-nx/2; i<=nx/2; i++){
		for(int j=-mx/2; j<=mx/2; j++){
			gx += I[row+i][col+j] * Kx[nx/2+i][mx/2+j];
		}
	}
	for(int i=-ny/2; i<=ny/2; i++){
		for(int j=-my/2; j<=my/2; j++){
			gy += I[row+i][col+j] * Ky[ny/2+i][my/2+j];
		}
	}
	G = sqrt(gx*gx + gy*gy);
	if(G > 255) G = 255.0;

	fprintf(fout, "(%d, %d): %f\n", row, col, G);

	fclose(fout);
}
void sort_file(int ni, int mi){
	FILE* fsort;
	int row, col;
	double G;

	if((fsort = fopen("ML2-output.txt", "r")) == NULL){
		perror("fopen-sort");
		exit(1);
	}
	double Isort[ni][mi];
	for(int i=1; i<ni-1; i++){
		for(int j=1; j<mi-1; j++){
			fscanf(fsort, "(%d, %d): %lf\n", &row, &col, &G);
			Isort[row][col] = G;
			printf("(%d, %d): %f\n", row, col, Isort[row][col]);
		}
	}
	fclose(fsort);

	if((fsort = fopen("ML2-output.txt", "w")) == NULL){
		perror("fopen-sorted");
		exit(1);
	}
	for(int i=1; i<ni-1; i++){
		for(int j=1; j<mi-1; j++){
			fprintf(fsort, "(%d, %d): %lf\n", i, j, Isort[i][j]);
		}
	}
	fclose(fsort);
}

int main(){
	struct msgbuf buf;
	int msqid;
	key_t key;
	FILE* fin;
	int child_count = 0;
	pid_t pid;

	if((fin = fopen("ML2-input.txt", "r")) == NULL){
		perror("fopen-in");
		exit(1);
	}

	fscanf(fin, "%d %d", &nx, &mx); // Kx
	int Kx[nx][mx];
	for(int i=0; i<nx; i++){
		for(int j=0; j<mx; j++){
			fscanf(fin, "%d", &Kx[i][j]);
		}
	}

	fscanf(fin, "%d %d", &ny, &my); // Ky
	int Ky[ny][my];
	for(int i=0; i<ny; i++){
		for(int j=0; j<my; j++){
			fscanf(fin, "%d", &Ky[i][j]);
		}
	}

	fscanf(fin, "%d %d", &ni, &mi); // I
	int I[ni][mi];
	for(int i=0; i<ni; i++){
		for(int j=0; j<mi; j++){
			fscanf(fin, "%d", &I[i][j]);
		}
	}

	fclose(fin);

	printf("%d %d %d %d %d %d\n", nx, mx, ny, my, ni, mi);

	if ((key = ftok("qa.c", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644)) == -1) {
		perror("msgget");
		exit(1);
	}
	
	printf("spock: ready to receive messages, captain.\n");

	for(int i=1; i<=256; i++){
		for(int j=1; j<=256; j++){
			if (msgrcv(msqid, (struct msgbuf*)&buf, sizeof(buf), 0, 0) == -1) {
				perror("msgrcv");
				exit(1);
			}
			if(child_count == MAX_CHILDREN){
				// break;
				wait(NULL);
				child_count--;
			}
			if((pid = fork()) == -1){
				perror("fork");
				exit(1);
			}
			else if(pid == 0){
				computeEdgeValue(buf.row, buf.col, Kx, Ky, I, nx, mx, ny, my, ni, mi);
				exit(1);
			}
			else{
				child_count++;
			}
			printf("%d %d\n", buf.row, buf.col);
		}
	}

	sort_file(ni, mi);
	
}
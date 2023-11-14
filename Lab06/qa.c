#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IMG_SIZE 256

struct msgbuf {
	long mtype;
	int row;
	int col;
};

int main(){
	struct msgbuf buf;
	int msqid;
	key_t key;

	if ((key = ftok("qa.c", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		perror("msgget");
		exit(1);
	}
	

	buf.mtype = 1;
	printf("%d %d\n", buf.row, buf.col);
	for(int i=1; i<=IMG_SIZE; i++){
		for(int j=1; j<=IMG_SIZE; j++){
			buf.row = i;
			buf.col = j;
			printf("%d %d\n", buf.row, buf.col);
			if (msgsnd(msqid, (struct msgbuf*)&buf, sizeof(buf), 0) == -1){
				perror("msgsnd");
				exit(1);
			}
		}
	}

	sleep(5);

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
}
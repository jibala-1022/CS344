#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

#define SLEEP_TIME 10

void start_daemon(){
    umask(0);
    
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid > 0) {
        exit(0);
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(1);
    }

    if(chdir("/") < 0){
        perror("chdir failed");
        exit(1);
    }

    for(int fd = getdtablesize(); fd>=0; fd--){
        close(fd);
    }

    openlog("mylog1", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Daemon created\n");

    int n = getpid();
    while(n != 1){
        syslog(LOG_NOTICE, "%d", n);
        if(n%2) n = 3*n+1;
        else n = n/2;
    }
    syslog(LOG_NOTICE, "%d", n);
    while(1){}

    // pid_t cpid;
    // while(1){
    //     cpid = fork();
    //     if (cpid < 0) {
    //         perror("Fork child failed");
    //         exit(1);
    //     }
    //     if(cpid == 0){
    //         char* sequence;
    //         int n = getpid();
    //         while(n != 1){
    //             sequence += n.to_string();
    //             if(n%2) n = 3*n+1;
    //             else n = n/2;
    //         }
    //         syslog(LOG_NOTICE, sequence);
    //         exit(0);
    //     }
    //     else{
    //         sleep(SLEEP_TIME);
    //     }
    // }
}

int main() {
    start_daemon();

    return 0;
}

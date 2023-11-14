// To start: gcc 210101028_Lab03.c -o run_daemon && ./run_daemon start
// To stop:  gcc 210101028_Lab03.c -o run_daemon && ./run_daemon stop
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

#define LOG_FILE "mylog"
#define SLEEP_TIME 20

// Function to start the daemon
void start_daemon(){
    pid_t pid;
    pid = fork();
    
    // Fork error handling
    if (pid < 0) {
        perror("fork 1 failed");
        exit(1);
    }
    
    // Parent process exits
    if (pid > 0) {
        exit(0);
    }

    struct rlimit rl;
    
    // Get resource limit information
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0){
        perror("rlimit failed");
        exit(0);
    }

    umask(0);
    
    // Create another child process
    pid = fork();

    // Fork error handling
    if (pid < 0) {
        perror("fork 2 failed");
        exit(1);
    }

    // Parent process writes the child process's PID to a file and exits
    if (pid > 0) {
        FILE* f = fopen("pid.txt", "w");
        fprintf(f, "%d", pid);
        fclose(f);
        exit(0);
    }

    // Create a new session
    if (setsid() < 0) {
        perror("setsid failed");
        exit(1);
    }

    // Change the working directory to the root directory
    if(chdir("/") < 0){
        perror("chdir failed");
        exit(1);
    }

    // If the maximum file descriptor limit is unlimited, set it to 1024
    if(rl.rlim_max == RLIM_INFINITY){
        rl.rlim_max = 1024;
    }
    
    // Close all open file descriptors
    for(int i=0; i<rl.rlim_max; i++){
        close(i);
    }

    // Open syslog for logging
    openlog(LOG_FILE, LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Daemon %d created", getpid());

    pid_t cpid;
    
    // Create child processes in a loop
    while(1){
        cpid = fork();
        
        // Fork error handling
        if (cpid < 0) {
            perror("Fork child failed");
            exit(1);
        }
        
        // Child process logic
        if(cpid == 0){
            long n = getpid();
            while(n != 1){
                syslog(LOG_NOTICE, "%ld", n);
                if(n % 2) n = 3 * n + 1;
                else n = n / 2;
            }
            syslog(LOG_NOTICE, "%ld", n);
            exit(0);
        }
        
        // Sleep between creating child processes
        sleep(SLEEP_TIME);
    }
}

// Function to stop the daemon
void stop_daemon(){
    pid_t pid;
    
    // Read the PID from the file
    FILE* f = fopen("pid.txt", "r");
    fscanf(f, "%d", &pid);
    fclose(f);

    // Send a SIGTERM signal to the daemon process
    if(kill(pid, SIGTERM) < 0){
        perror("killing failed");
        exit(1);
    }
    
    // Log that the daemon has been killed
    syslog(LOG_NOTICE, "Daemon %d killed", pid);
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        perror("Usage: ./run_daemon <start/stop>");
        return 1;
    }

    if(strcmp("start", argv[1]) == 0){
        printf("Starting Daemon...\n");
        start_daemon();
    }
    else if(strcmp("stop", argv[1]) == 0){
        printf("Stopping Daemon...\n");
        stop_daemon();
    }
    else{
        printf("Invalid Command\n");
    }

    return 0;
}

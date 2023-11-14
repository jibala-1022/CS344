// gcc compute_sequence_4.c -o compute_sequence_4 && ./compute_sequence_4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>

#define BUFFER_SIZE 1024
#define READ 0
#define WRITE 1

int main()
{
    char input[100],command1[100],command2[100],command3[100];
    char ch;
    int i=0;
    while(1)
    {
        while((ch=getchar())!='\n')
        {
            input[i++]=ch;
        }
        input[i]='\0';
        if(strcmp(input, "quit")==0)
        exit(0);
        int j=0;
        while(input[j]!='|')
        {
            command1[j]=input[j];
            j++;
        }
        command1[j]='\0';
        int k,l;
        k=0;
        j++;
        while(input[j]!='|')
        {
            command2[k++]=input[j++];
        }
        command2[k]='\0';
        j++;
        k=0;
        while(input[j]!='\0')
        {
            command3[k++]=input[j++];
        }
        command3[k]='\0';

        int fd[2];
        pid_t pid;

        int _out,_in;

        dup2(1,_out);
        dup2(0,_in);

        if(pipe(fd)==-1)
        {
            fprintf(stderr,"Pipe failed");
            return 1;
        }

        pid=fork();

        if(pid<0)
        {
            fprintf(stderr,"Fork failed");
            return 1;
        }

        if(pid>0)
        {
            close(fd[READ]);

            if (dup2(fd[WRITE], 1) == -1) {
                perror("dup2");
                return 1;
            }

            system(command1);

            dup2(_out,1);

            close(fd[WRITE]);

            wait(NULL);

        }
        else
        {
            close(fd[WRITE]);

            dup2(fd[READ],0);

            int fd1[2];

            pid_t pid1;                

                if(pipe(fd1)==-1)
                {
                    fprintf(stderr,"Pipe failed");
                    return 1;
                }

                pid1=fork();

                if(pid1<0)
                {
                    fprintf(stderr,"Fork failed");
                    return 1;
                }

                else if(pid1>0)
                {
                    close(fd1[READ]);

                    if (dup2(fd1[WRITE], 1) == -1) {
                        perror("dup2");
                        return 1;
                    }

                    system(command2);

                    dup2(_out,1);

                    close(fd1[WRITE]);

                    wait(NULL);

                }
                else
                {
                    close(fd1[WRITE]);

                    dup2(fd1[READ],0);

                    system(command3);

                    dup2(_in,0);

                    close(fd1[READ]);

                    exit(0);
                }

            dup2(_in,0);
            close(fd1[READ]);
            exit(0);
        }
        i=0;
    }
    
    return 0;
}
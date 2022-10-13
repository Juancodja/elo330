#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


int done = 0;
void stop(int signum){
    done = 1;
    //printf("alarm\n");
}

int main(int argc, char *argv[]){
    int fd, n = 1, i;
    int N = atoi(argv[2]);
    int buff[2] = {-1, 1};
    int endff[] = {0};
    int to_be_read = 2*sizeof(int);

    const char* name = argv[1];
    
    fd = open(name, O_WRONLY);
    
    signal(SIGALRM, stop);

    alarm(N);

    while(!done){
        for(i = 0; i<n; i++){
            write(fd, buff, to_be_read);
        }
        n++;
    }
    int a = write(fd, endff, sizeof(int));
    //printf("wrote %i bytes\n", a);
    close(fd);
    return 0;
}
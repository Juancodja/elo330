#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define CMILI 100000

/* Variables goblales para uso de get_rate*/
static int prev_sum_read = 0;
int sum_read = 0; // solo escribe main
int *rates;
int calls = 0;

static void get_rate(int signum){
    rates[calls] = sum_read-prev_sum_read;
    prev_sum_read = sum_read;
    calls++;
    //printf("%i\n", prev_sum_read);
    
}

int main(int argc, char *argv[]){
    int fd, n, sum = 0;
    int buff[2] = {1, 0};
    int N = atoi(argv[2]);
    int to_be_read = 2*sizeof(int);

    rates = (int *) malloc(15*N*sizeof(int));

    const char* name = argv[1];

    /* Eliminar fifo en caso de que exista */
    unlink(name);

    mkfifo(name, 0666);

    fd = open(name, O_RDONLY);

    signal(SIGALRM, get_rate);

    ualarm(CMILI, CMILI);

    
    while(1){
        n = read(fd, buff, to_be_read);
        sum_read+= n;
        if(!buff[0]) break;
        sum = sum + buff[0] + buff[1];
    }
    ualarm(0,0);
    rates[calls] = sum_read-prev_sum_read;
    calls++;
    //printf("buff[0] = %i\n", buff[0]);
    //printf("\nSuma = %i\n", sum);

    printf("#interval\tbit\n");
    int i;
    for(i = 0 ; i < calls; i++){
        printf("%i\t%i\n", 100*i, 10*rates[i]);
    }
    printf("\n");



    close(fd);
    free(rates);
    unlink(name);
    return 0;
}
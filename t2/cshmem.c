#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#define CMILI 100000

/* Variables goblales para uso de get_rate*/
static int prev_sum_read = 0;
int sum_read = 0; // solo escribe main
int *rates;
int calls = 0;

static void get_rate(int signum){
    rates[calls] = sizeof(int)*(sum_read-prev_sum_read);
    prev_sum_read = sum_read;
    calls++;
}



int main(int argc, char *argv[]){
    int fd, n = 2, sum = 0, i, m = 0;
    int N = atoi(argv[2]);
    void *memptr;
    const SIZE = 524288;
    const char *nameEmpty = "/sempaforo_vacio";
    const char *nameFull = "/semaforo_lleno";
    sem_t * empty_sem, *full_sem;

    rates = (int *) malloc(15*N*sizeof(int));

    const char* name = argv[1];

    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        printf("cons: Shared memory failed: %s\n", strerror(errno));
        exit(1);
    }
    
    if (ftruncate(fd, SIZE) == -1){
        printf("cons: Truncate failed %s\n", strerror(errno));
        exit(1);
    }

    memptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (memptr == MAP_FAILED) {
    printf("cons: Map failed: %s\n", strerror(errno));
    exit(1);
    }

    int* ptr = (int *) memptr;

    // first remove the semaphore if it already exists
    /*
    if (sem_unlink(nameEmpty) == -1)
        printf("Error removing %s: %s\n", nameEmpty, strerror(errno));
    if (sem_unlink(nameFull) == -1)
        printf("Error removing %s: %s\n", nameFull, strerror(errno));
        */
    sem_unlink(nameEmpty);
    sem_unlink(nameFull);
    

    // create and initialize the semaphore
    if ( (empty_sem = sem_open(nameEmpty, O_CREAT, 0666, 1)) == SEM_FAILED)
        printf("Error creating %s: %s\n", nameEmpty, strerror(errno));
    if ( (full_sem = sem_open(nameFull, O_CREAT, 0666, 0)) == SEM_FAILED)
        printf("Error creating %s: %s\n", nameFull, strerror(errno));

    signal(SIGALRM, get_rate);

    ualarm(CMILI, CMILI);

    while(1){
        sem_wait(full_sem);
        if (*ptr == 0) break;
        for(i = 0 ; i<n ; i++){
            sum += *(ptr + (m+i));
        }
        m = n;
        sum_read += n;
        n+2;
        sem_post(empty_sem);
    }
    ualarm(0,0);
    rates[calls] = sizeof(int)*(sum_read-prev_sum_read);
    calls++;
    
    printf("\nSuma = %i\n", sum);

    for(i = 0 ; i < calls/5 ; i++){
        for(n=0 ; n < 5; n++){
            printf("%i ", rates[5*i+n]);
        }
        printf("\n");
    }
    for(n=i*5 ; n < calls ; n++){
        printf("%i ", rates[n]);
    }
    printf("\n");


    sem_close(empty_sem);
    sem_unlink(nameEmpty);
    sem_close(full_sem);  
    sem_unlink(nameFull);

    /* remove the mapped shared memory segment from the address space of the process */
    if (munmap(memptr, SIZE) == -1) {
        printf("cons: Unmap failed: %s\n", strerror(errno));
        exit(1);
    }

    /* close the shared memory segment as if it was a file */
    if (close(fd) == -1) {
        printf("cons: Close failed: %s\n", strerror(errno));
        exit(1);
    }

    /* remove the shared memory segment from the file system */
    if (shm_unlink(name) == -1) {
        printf("cons: Error removing %s: %s\n", name, strerror(errno));
        exit(1);
    }

    free(rates);

}
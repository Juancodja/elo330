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

int done = 0;
void stop(int signum){
    done = 1;
}

int main(int argc, char *argv[]){
    int fd, n = 1, i, m= 0;
    int N = atoi(argv[2]);
    void* memptr;
    const int SIZE = 524288;
    const char *nameEmpty = "/sempaforo_vacio";
    const char *nameFull = "/semaforo_lleno";
    sem_t * empty_sem, *full_sem;

    const char* name = argv[1];
    usleep(200);

    fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        printf("prod: Shared memory failed: %s\n", strerror(errno));
        exit(1);
    }

    /* map the shared memory segment to the address space of the process */
    memptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memptr == MAP_FAILED) {
        printf("prod: Map failed: %s\n", strerror(errno));
        // close and shm_unlink?
        exit(1);
    }

    int * ptr = (int *) memptr;

    // create and initialize the semaphore
    if ( (empty_sem = sem_open(nameEmpty, 0)) == SEM_FAILED)
        printf("Error opening %s: %s\n",nameEmpty, strerror(errno));
    if ( (full_sem = sem_open(nameFull, 0)) == SEM_FAILED)
        printf("Error opening %s: %s\n",nameFull, strerror(errno));

        
    signal(SIGALRM, stop);

    alarm(N);

    while(!done){
        sem_wait(empty_sem);
        for(i = 0; i<n; i++){
            *(ptr + i*2) = -1;
            *(ptr + (i*2+1)) = 1;
            //printf("%p\n", (void *) (ptr + i*2));
            //printf("%p\n", (void *) (ptr + (i*2+1)));
        }
        n++;
        //printf("n = %i\n", n);
        sem_post(full_sem);
    }

    sem_wait(empty_sem);
    *(ptr) = 0;
    sem_post(full_sem);

    sem_close(empty_sem);
    sem_close(full_sem);
    

    /* remove the mapped memory segment from the address space of the process */
    if (munmap(memptr, SIZE) == -1) {
        printf("prod: Unmap failed: %s\n", strerror(errno));
        exit(1);
    }


    /* close the shared memory segment as if it was a file */
    if (close(fd) == -1) {
        printf("cons: Close failed: %s\n", strerror(errno));
        exit(1);
    }

    return 0;
}
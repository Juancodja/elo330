#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#define MAX_PLAYERS 6
#define MAX_HEIGHT 700
#define MAX_WIDTH 1200
#define MAX_COLOR 255



#ifndef PLAYERSTRUCT
#define PLAYERSTRUCT

typedef struct{
    char name[21];      /* 20 char max name length */
    int posX;
    int posY;
    int color[3];       /* [r, g, b] */
    int alive;
    int active;
} player_entity;

extern player_entity playerArr[MAX_PLAYERS];

#endif



#ifndef FILESHEADER
#define FILESHEADER

/* Files to store and send info */
extern FILE *playersFile;
extern FILE *bulletsFile;

int openfiles(void);
int writePlayerInfo(int);
int beginPlayerInfo(void);

#endif

#ifndef PROCESSINGHEADER
#define PROCESSINGHEADER

void advanceTest(int);

#endif


#ifndef SEMHEADER
#define SEMHEADER

extern sem_t *playerSem;

#endif
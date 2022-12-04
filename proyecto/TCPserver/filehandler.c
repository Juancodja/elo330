#include "serverlib.h"

/* Files to store and send info */
FILE *playersFile;
FILE *bulletsFile;

int openfiles(void){
    const char pFileName[] = "players.json\0";
    const char bFileName[] = "bullets.json\0";
    playersFile = fopen(pFileName, "wb");
    bulletsFile = fopen(bFileName, "wb");

    if(playersFile == NULL || bulletsFile == NULL){
        perror("fopen");
        return -1;
    }
    return 0;
}


int beginPlayerInfo(void){
    if(fprintf(playersFile, "{\n\t\"Players\": [") < 0){
        perror("beginig player file");
        return -1;
    }
    return 0;
}

int writePlayerInfo(int id){
    if(fprintf(playersFile, "\n\t\t{\"name\": \"%s\", \"posX\": %i, \"posY\": %i, \"R\": %i, \"G\": %i, \"B\": %i, \"alive\": %i},", playerArr[id].name,
    playerArr[id].posX, playerArr[id].posY, playerArr[id].color[0],
    playerArr[id].color[1], playerArr[id].color[2], playerArr[id].alive) < 0){
        perror("writing player info");
        return -1;
    }
    return 0;
}

int endPlayerInfo(void){

    if(fprintf(playersFile, "\n\t]\n}") < 0){
        perror("ending player info");
        return -1;
    }
    return 0;
}



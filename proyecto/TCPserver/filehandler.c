#include "serverlib.h"

/* Files to store and send info */
FILE *playersFileW;
FILE *bulletsFile;
FILE *playersFileR;

const char pFileName[] = "players.json\0";
const char bFileName[] = "bullets.json\0";

int openfiles(void){
    playersFileW = fopen(pFileName, "wb");
    bulletsFile = fopen(bFileName, "wb");

    if(playersFileW == NULL || bulletsFile == NULL){
        perror("fopen");
        return -1;
    }
    return 0;
}


int beginPlayerInfo(void){
    if(fprintf(playersFileW, "{\n\t\"Players\": [") < 0){
        perror("beginig player file");
        return -1;
    }
    return 0;
}

int writePlayerInfo(int id){
    if(fprintf(playersFileW, "\n\t\t{\"name\": \"%s\", \"posX\": %i, \"posY\": %i, \"R\": %i, \"G\": %i, \"B\": %i, \"alive\": %i},", playerArr[id].name,
    playerArr[id].posX, playerArr[id].posY, playerArr[id].color[0],
    playerArr[id].color[1], playerArr[id].color[2], playerArr[id].alive) < 0){
        perror("writing player info");
        return -1;
    }
    return 0;
}

int endPlayerInfo(void){

    if(fprintf(playersFileW, "\n\t]\n}\n") < 0){
        perror("ending player info");
        return -1;
    }
    return 0;
}

FILE *open_p_for_read(){
    playersFileR = fopen(pFileName, "rb");

    if(playersFileR == NULL){
        perror("fopen");
    }
    return playersFileR;
}

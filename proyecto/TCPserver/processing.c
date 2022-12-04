#include "serverlib.h"
#define STEP 10

void advanceTest(int id){     /* seccion critica */
    if((playerArr[id].posX += STEP) > MAX_WIDTH)
        playerArr[id].posX = 0;
    
    if((playerArr[id].posY += STEP) > MAX_HEIGHT)
        playerArr[id].posY = 0;
    
    if((playerArr[id].color[0] += STEP) > MAX_COLOR)
        playerArr[id].color[0] = 0;
    
    if((playerArr[id].color[1] += STEP) > MAX_COLOR)
        playerArr[id].color[1] = 0;
    
    if((playerArr[id].color[2] += STEP) > MAX_COLOR)
        playerArr[id].color[2] = 0;
    
    return;
}


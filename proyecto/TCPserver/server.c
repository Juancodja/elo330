#include "serverlib.h"
#define DELAY 100000

/*
    Main thread processes game
    Secondary thread accepts TCP connections then creates threads to serve them
*/



void *server(void *);
void *connected_player(void* );
void send_Pinfo(int);

player_entity playerArr[MAX_PLAYERS];

sem_t *playerSem;


/*  
    Implementar semaforo o varcond para el while del servidor
    con inicializacion MAX_PLAYERS, decremento cuando
    conecta un jugador e incremento cuando desconecta
*/


void *connected_player(void* param){
    printf("Player connected.\n");
    int sock_player = *(int *) param;
    int i, k;
    
    sem_wait(playerSem);
    for(i = 0 ; i < MAX_PLAYERS ; i++){         /* seccion critica */
        printf("in for cicle.\n");
        printf("active: %i\n", playerArr[i].active);
        if(playerArr[i].active == 1) continue;

        printf("Asigned player number\n");
        sprintf(playerArr[i].name, "Player%i\0", i);
        playerArr[i].posX = 200;
        playerArr[i].posY = 200;
        for(k = 0 ; k < 3 ; k++){
            playerArr[i].color[k] = 0;
        }
        playerArr[i].alive = 1;
        playerArr[i].active = 1;
        break;
    }
    sem_post(playerSem);
    
    printf("nombre: %s\nposX: %i\nposY: %i\nactivo: %i\n", playerArr[i].name,
    playerArr[i].posX, playerArr[i].posY, playerArr[i].active);

    while(1){
        usleep(DELAY);
        sem_wait(playerSem);
        send_Pinfo(sock_player);
        sem_post(playerSem);
/*        printf("nombre: %s\nposX: %i\nposY: %i\nactivo: %i\nR: %i\nG: %i\n B: %i\n", playerArr[i].name,
        playerArr[i].posX, playerArr[i].posY, playerArr[i].active, playerArr[i].color[0],
        playerArr[i].color[1], playerArr[i].color[2]);  */
    }

}


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Error: Too few arguments\nUsage: %s <Port>\n", argv[0]);
        exit(1);
    }

    int i;
    long off = 0;
    int writtenPinfo = 1;

    /* open files */
    if(openfiles() != 0){
        printf("open files: couldn't open files\n");
        exit(-1);
    }

    /* initialize active member of player struct */
    for (i = 0 ; i < MAX_PLAYERS ; i++){
        playerArr[i].active = 0;
    }

    /* player struct semaphore */
    const char *nameSem = "/PlayerSem";
    sem_unlink(nameSem);
    if ( (playerSem = sem_open(nameSem, O_CREAT, 0666, 1)) == SEM_FAILED)
        perror("creating sem");



    char port[6];
    strcpy(port, argv[1]);

    pthread_t server_t;

    if (pthread_create(&server_t, NULL, server, (void *)port) != 0){
        perror("server thread");
        exit(2);
    }

    /* Processing thread begins here */
    while(1){
        usleep(DELAY);
        sem_wait(playerSem);
        if(beginPlayerInfo() != 0)
            exit(1);
        for(i = 0 ; i < MAX_PLAYERS ; i++){
            if(playerArr[i].active == 1){
                advanceTest(i);              /* seccion critica */
                if(writePlayerInfo(i) != 0)
                    exit(1);
                writtenPinfo = 1;
            }
        }
        if(writtenPinfo)                         /* if player info was written */
            fseek(playersFileW, -1, SEEK_CUR);    /* deletes comma */
        if(endPlayerInfo() != 0){
            printf("revisar este exit\n");
            exit(1);
        }
        fseek(playersFileW, off, SEEK_SET);      /* resets file pointer */
        sem_post(playerSem);
    }


    pthread_join(server_t, NULL);




}


void *server(void *param){
    int sock, new_sock, i;
    
    char *port = (char *) param;

    pthread_t players[MAX_PLAYERS];

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* Config hint struct to get addr info */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;          /* IPv4 or IPv6*/
    hints.ai_socktype = SOCK_STREAM;      /* TCP */
    hints.ai_flags = AI_PASSIVE;          /* For wildcard IP address */
    hints.ai_protocol = 0;                /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;    

    if(getaddrinfo(NULL, port, &hints, &result) != 0){
        perror("getaddinfo");
        exit(1);
    }        
    
    for (rp = result; rp != NULL; rp = rp->ai_next){
        
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        
        if (sock == -1) continue;

        if(bind(sock, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                      /* SUCCESS */

        close(sock);
        
    }

    freeaddrinfo(result);

    if(rp == NULL){
        perror("bind");
        exit(1);
    }

    if (listen(sock, MAX_PLAYERS) < 0){
        perror("listen");
        exit(1);
    }

    
    i = 0;
    while(1){
        new_sock = accept(sock, rp->ai_addr, &(rp->ai_addrlen));

        if(pthread_create(&players[i++], NULL, connected_player, &new_sock) != 0){
            perror("pthread");
            exit(1);
        }
        if( i >= MAX_PLAYERS)
        {
            i = 0;
            while(i < MAX_PLAYERS){
                pthread_join(players[i++], NULL);
            }
            i = 0;
        }

    }

}


void send_Pinfo(int s){         /* seccion critica */
    FILE *playersRead;
    char data[SEND_SIZE];

    if((playersRead = open_p_for_read()) == NULL){
        exit(1);
    }
    
    while(fgets(data, SEND_SIZE, playersRead) != NULL) {
        if (send(s, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        printf("%s", data);
        bzero(data, SEND_SIZE);
    }
    //write(s, playersRead, 500);

    return;
}
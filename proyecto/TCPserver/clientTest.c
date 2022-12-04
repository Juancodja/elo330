#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if (argc < 3){
       printf("Error: Too few arguments.\nUsage: %s <Server> <Port>\n", argv[0]);
       exit(1);
    }

    char hostname[64], port[6], buff[200];
    int s;
    char A = 'A', B = 'B';

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    /* Configuracion de hints, para recuperacion de addrinfo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */
    hints.ai_flags = AI_ADDRCONFIG;     /* Return only existing IPv4 or IPv6 */
    hints.ai_protocol = 0;              /* Any protocol */

    strcpy(hostname, argv[1]);
    strcpy(port, argv[2]);

    if(getaddrinfo(hostname, port, &hints, &result) != 0){
        perror("getaddinfo");
        exit(1);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next){
        s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (s == -1) continue;

        if(connect(s, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                      /* SUCCESS */
        
        close(s);
    }

    freeaddrinfo(result);

    if(rp == NULL){
        printf("Error: Could not connect\n");
        exit(1);
    }

    while(recv(s, (void *)buff, sizeof(buff), 0)){
        write(STDIN_FILENO, buff, sizeof(buff));
    }
    close(s);
    exit(0);
}
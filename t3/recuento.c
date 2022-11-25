#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
int send_vote(int __fd, const void* __buf, ssize_t __n, int __flags){
    char votoA[1024] = "a", votoB[1024] = "b";
    if(strcmp(__buf, votoA) == 0){
        send(__fd, __buf, __n, __flags);
        return 0;
    }
    else if(strcmp(__buf, votoB) == 0){
        send(__fd, __buf, __n, __flags);
        return 0;
    }
    else if(strcmp(__buf, "f") == 0){
        printf("Finalizando Votación.\n");
        return -1;
    }
    else{
        printf("Voto incorrecto opciones {a, b}\n");
        return 0;
    }
}
*/

void flag(){
    printf("flag\n");
}

void err_exit(const char* msg){
    printf("Error: %s\n", msg);
    exit(1);
}

void err_usage(const char* msg){
    printf("Usage: %s\n", msg);
}

int input_len(const char* input){
    int i = 0; 
    while(input[i] != '\n'){
        i++;
    }
    return i;
}
int main(int argc, char * argv[])
{
    int n, s, len;
    char inputchar;
    char buf[1024];
    char hostname[64];
    unsigned int portnumber = 47200;
    struct hostent *hp;
    struct sockaddr_in name;

    switch (argc)
    {
    case 1:
        gethostname(hostname, sizeof(hostname));
        break;
    case 2:
        strcpy(hostname, argv[1]);
        break;
    case 3:
        strcpy(hostname, argv[1]);
        portnumber = atoi(argv[2]);
        break;
    default:
        break;
    }
    
    hp = gethostbyname(hostname);
    if(hp == NULL){
        err_exit("get host failed");
    }

    s = socket(AF_INET, SOCK_STREAM, 0);

    name.sin_family = AF_INET;
    name.sin_port = htons(portnumber);

    memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);

    len = sizeof(struct sockaddr_in);

    connect(s, (struct sockaddr *) &name, len);


    while ((n = read(0, buf, sizeof(buf))) > 0) {
        if(input_len(buf)==1){
            inputchar = buf[0];
            if(inputchar == 'a' || inputchar == 'b'){
                if (send(s, buf, n, 0) < 0) {
                    err_exit("send failed");
                }    
            }
            else if(inputchar == 'f'){
                break;
            }            
            else{
                err_usage("caracteres validos {a, b, f}");
            }
        }
        else(err_usage("escriba solo un caracter como input."));
    }

    close(s);
    exit(0);
}
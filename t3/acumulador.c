#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


#define PORTNUMBER  47200
#define N_THREADS       5 

void* handler(void* args);

int main(int argc, char* argv[])
{
    char buf[10];
    int s, n , ns;
    unsigned int portnumber;
    socklen_t addr_len;
    struct sockaddr_in name;
    
    pthread_t handler_thread;
    pthread_create(&handler_thread, NULL, handler, NULL);
    pthread_join(handler_thread, NULL);

    switch (argc)
    {
    case 1:
        portnumber = PORTNUMBER;
        break;
    case 2:
        portnumber = atoi(argv[1]);
        break;
    default:
        break;
    }



    s = socket(AF_INET, SOCK_STREAM, 0);

    name.sin_family = AF_INET;
    name.sin_port = htons(portnumber);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_len = sizeof(struct sockaddr_in);

    if( bind(s, (struct sockaddr *) &name, addr_len))
		printf("bind error");

    listen(s, 5);

    for( ; ; ){
        fflush(stdout);
        ns = accept(s, (struct sockaddr *)&name, &addr_len);
        printf("Connection from : %s\n",inet_ntoa(name.sin_addr));

            while ((n = recv(ns, buf, sizeof(buf), 0)) > 0)
            write(1, buf, n);
    }
    close(ns);
    close(s);
    exit(0);
    
}

void* handler(void* arg){
    printf("Soy un PeHilo\n");

    return (void *) NULL;
}
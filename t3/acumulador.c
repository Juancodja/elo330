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

struct vote_count
{
    int a;
    int b;
};

struct msg_handler_args
{
    int client_socket;
    char client_addr[64];
    struct vote_count* votes;
};


void* handle_vote(void* args);

void err_exit(const char* msg);


pthread_mutex_t lock;


int main(int argc, char* argv[])
{
    char buf[10];
    int s, client_socket;
    unsigned int portnumber;
    socklen_t addr_len;
    struct sockaddr_in name;
    
    pthread_t handler_thread;

    struct vote_count votes;
    struct msg_handler_args args;

    votes.a = 0; 
    votes.b = 0;

    args.votes = &votes;

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

    if( bind(s, (struct sockaddr *) &name, addr_len)){
		err_exit("bind failed");
    }
    if(listen(s, 5)){
		err_exit("listed failed");
    };

    for( ; ; ){
        fflush(stdout);
        client_socket = accept(s, (struct sockaddr *)&name, &addr_len);
        strcpy(args.client_addr,inet_ntoa(name.sin_addr));
        args.client_socket = client_socket;
        pthread_create(&handler_thread, NULL, handle_vote, (void *)&args);

    }
    close(client_socket);
    close(s);
    exit(0);
    
}


void* handle_vote(void* args){
    char buf[10];
    char client_addr[64]; 
    int n, client_socket; 
    struct vote_count* votes;

    strcpy(client_addr, ((struct msg_handler_args*)args)->client_addr);
    client_socket = ((struct msg_handler_args*)args)->client_socket; 
    votes = ((struct msg_handler_args*)args)->votes; 
    
    

    printf("Connection from : %s\n",client_addr);

        while ((n = recv(client_socket, buf, sizeof(buf), 0)) > 0){
            if(buf[0]=='a'){    
                pthread_mutex_lock(&lock);
                votes->a = votes->a + 1;
                pthread_mutex_unlock(&lock);
            }
            if(buf[0]=='b'){
                pthread_mutex_lock(&lock);
                votes->b = votes->b + 1;
                pthread_mutex_unlock(&lock);    
            }
            printf("votos A: %i, votos B: %i\n", votes->a, votes->b);
        }

    printf("Connection from : %s finished\n",client_addr);
    close(client_socket);
    return (void *) NULL;
}

void err_exit(const char* msg){
    printf("Error: %s\n", msg);
    exit(1);
}
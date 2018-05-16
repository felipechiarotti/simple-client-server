#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>

void* handle_client(void *arg){
    char buffer_do_cliente[256];
    int sockEntrada = *(int *)arg;
    for(;;){
        read(sockEntrada, buffer_do_cliente, sizeof(buffer_do_cliente));
        if(strcmp(buffer_do_cliente, "desconectar") != 0){
            printf("Cliente %d: %s\n", sockEntrada-3, buffer_do_cliente);
        }else{
            printf("%d desconectado!", sockEntrada);
            close(sockEntrada);
            pthread_exit((void*) 0);
        }
    }
}

int start_server(){
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    if(sockfd < 0){
        printf("Erro no Socket\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5222);
    if(bind(sockfd, (struct sockaddr *) & server_addr, sizeof(server_addr)) < 0){
        printf("Erro no Socket\n");
    }
    if(listen(sockfd,0) < 0){
        printf("Erro no Socket: Limite de Clientes\n");
        exit(1);
    }
    return sockfd;
}

int main(){
    printf("Inicializando o Servidor!\n");
    struct sockaddr_in server_addr;
    int sockfd = start_server();
    printf("Servidor Inicializado....\n\n");
  while(1){
        int clienteSockfd;
        struct sockaddr_in client_addr;
        unsigned int clntLen;
        clntLen = sizeof(client_addr);
        pthread_t thread;
        printf("Aguardando clientes...\n");
        if((clienteSockfd = accept(sockfd, (struct sockaddr *) & client_addr, &clntLen)) < 0){
            printf("Erro na conexão com cliente\n");
            exit(1);
        }
        if(pthread_create(&thread, NULL, handle_client, &clienteSockfd) != 0){
            printf("Erro na inicialização da Thread\n");
            exit(1);
        }
        printf("[!] Cliente Conectado\n\n");
        pthread_detach(thread);
    }

    exit(0);
}
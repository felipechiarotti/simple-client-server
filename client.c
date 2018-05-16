#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <string.h>
 
int connect_client(char* ip, int port){
    int clienteSockfd;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof (serv_addr);
 
    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("Erro no Socket\n");
        exit(1);
    }
    bzero((char *) & serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
   if(connect(clienteSockfd,(struct sockaddr *)&serv_addr, addrlen) < 0)
     {
        printf("Erro ao conectar-se ao servidor\n");
        exit(1);
    }
        return clienteSockfd;
}

void run_client(int clienteSockfd)
{
  char buffer_para_servidor[256];
  printf("Digite uma mensagem para o servidor: ");
   do {
 
        scanf("%s",buffer_para_servidor);
        fflush(stdin);
        write(clienteSockfd, buffer_para_servidor, sizeof (buffer_para_servidor));
    } while (strcmp(buffer_para_servidor, "sair") != 0);
    close(clienteSockfd);
}

int main(int argc, char* argv[]){
    struct sockaddr_in serv_addr;
    socklen_t addr_len = sizeof(serv_addr);
    int client_data = connect_client(argv[1],atoi(argv[2]));
    run_client(client_data);
}
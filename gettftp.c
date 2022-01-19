#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 512
#define PORT_SIZE 2

const char octet[] = "aocteta";

const char sep[] = ":";
//port 69
int main(int args, char **argv) {
    const struct addrinfo hints = {0};
    struct addrinfo *ai;
    int sock, log;
    ssize_t env, rec;
    printf("nik\n");

    int lencom = strlen(argv[2]);
    char req[lencom+8];
    req[0]=0;
    req[1]=1;
    strcpy(&req[2],argv[2]);
    /*
    char *ptr;
    char adresse[BUFF_SIZE];
    char port[PORT_SIZE];

    ptr = strtok(argv[1], sep);
    strcpy(adresse,ptr);
    strcpy(port,ptr);

    write(STDOUT_FILENO, adresse, strlen(adresse));
    write(STDOUT_FILENO, port, strlen(port));
    */
    write(STDOUT_FILENO, req, strlen(req));
    printf("nik\n");
    getaddrinfo(argv[1], NULL, &hints , &ai);

    printf("nik\n");
    if ((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    printf("nik\n");
    if ((log = connect(sock,ai->ai_addr,ai->ai_addrlen)) < 0){
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    else if (log == 0){
        printf("connecté");
    }
    printf("nik\n");
    if ((log = connect(sock,ai->ai_addr,ai->ai_addrlen)) < 0){
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    printf("nik\n");
    if ((env = send(sock,req, strlen(req), MSG_CONFIRM)) < 0){
        perror("send error");
        exit(EXIT_FAILURE);
    }
    printf("nik\n");
    printf("%ld",env);
    char buff[BUFF_SIZE];
    int lenb;
    printf("nik\n");
    if ((rec = recv(sock, buff, lenb, MSG_CONFIRM)) < 0){
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    printf("nik\n");
    printf("%ld",rec);
    printf("nik\n");
    return 0;
    //printf("nik");
}
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//10.10.32.11
//lili file
#define BUFF_SIZE 512
#define PORT_SIZE 2

const char octet[] = "octet";

const char sep[] = ":";
//port 69
int main(int args, char **argv) {
    const struct addrinfo hints = {0};
    struct addrinfo *ai;
    int sock;
    int log = -1;
    ssize_t env, rec;
    //printf("nik\n");

    int lencom = strlen(argv[2]);
    char *req = malloc(lencom+9*sizeof(char));
    req[0]=0;
    req[1]=1;
    strcpy(&req[2],argv[2]);
    req[lencom+2]=0;
    strcpy(&req[lencom+3],octet);
    req[lencom+8]=0;
    for (int i = 0; i < lencom+9; ++i) {
        printf("%c, ", req[i]);
    }
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
    //write(STDOUT_FILENO, req, strlen(req));
    //printf("nik\n");
    getaddrinfo(argv[1], NULL, &hints , &ai);

    if ((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    if ((log = connect(sock,ai->ai_addr,ai->ai_addrlen)) < 0){
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    else if (log == 0){
        printf("connecté\n");
    }
    printf("nik");
    if ((env = send(sock,req, strlen(req), MSG_CONFIRM)) < 0){
        perror("send error");
        exit(EXIT_FAILURE);
    }
    printf("%ld",env);
    printf("nik");
    char buff[BUFF_SIZE];
    int lenb=0;
    if ((rec = recv(sock, buff, lenb, MSG_CONFIRM)) < 0){
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    printf("%ld\n",rec);
    return 0;
    //printf("nik");
}
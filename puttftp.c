#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 64

int main(int args, char * * argv) {
    struct addrinfo hints;
    struct addrinfo *res;
    int sock;

    getaddrinfo(argv[1], SOCK_STREAM, &hints , &res);
    if (sock = socket(argv[1],SOCK_STREAM,IPPROTO_TCP) < 0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
}
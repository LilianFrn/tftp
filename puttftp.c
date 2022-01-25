#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATA_BUFF_SIZE 512 //taille max d'un paquet DATA
#define BUFF_SIZE 516 //taille max d'un paquet DATA + 4 octet de header
#define PORT "1069" //port tftp local
#define UNUSED(x) (void)(x)

const char octet[] = "octet"; //type de transfert

int main(int args, char **argv) {
    
    //variables
    struct addrinfo hints; //masque pour ai
    struct addrinfo *ai; //infos serveur de destination
    //adresse de réponse du serveur
    socklen_t serv_addr_len;
    struct sockaddr_storage serv_addr;

    char buff[BUFF_SIZE] = ""; //buffer pour les recv
    char data_buff[DATA_BUFF_SIZE] = ""; //buffer de lecture de fichier

    FILE *fp; //pointeur de fichier

    int sock; //int de test 
    int pack = 1; //compteur de paquet
    ssize_t env, rec; //int de longueur de message
    UNUSED(args);

    //définition du masque
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPV4
    hints.ai_socktype = SOCK_DGRAM; //Type de socket
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP; //protocole UDP

    //création de la commande WRQ
    int lencom = strlen(argv[2]);
    char *req = malloc(lencom+9*sizeof(char));
    req[0]=0;
    req[1]=2; //ID WRQ
    strcpy(&req[2],argv[2]); //nom du fichier
    req[lencom+2]=0;
    strcpy(&req[lencom+3],octet); //type de transfert
    req[lencom+8]=0;

    getaddrinfo(argv[1], PORT, &hints , &ai); //récupération des données serveur

    //Ouverture du socket
    if ((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    //Envoi de la requête WRQ
    if ((env = sendto(sock,req, lencom+9, 0,ai->ai_addr,ai->ai_addrlen)) < 0){
        perror("send error");
        exit(EXIT_FAILURE);
    }

    //Ouverture du fichier d'origine en mode read
    fp = fopen(argv[2], "r");

    do {
        serv_addr_len = sizeof(struct sockaddr_storage);

        //Réception de l'ACK du serveur
        if ((rec = recvfrom(sock, buff, BUFF_SIZE, 0,(struct sockaddr *) &serv_addr, &serv_addr_len)) < 0){
            perror("recv error");
            exit(EXIT_FAILURE);
        }
        buff[1] = 3; //Transformation en requête DATA
        buff[3] = pack; //Numérotation du paquet

        fread(data_buff,DATA_BUFF_SIZE,1,fp); //Lecture du fichier pour remplir data_buff
        strcpy(&buff[4],data_buff); //Ajout des data sur le header

        //Envoi de datas au serveur
        if ((env = sendto(sock,buff, strlen(data_buff)+4, 0,(struct sockaddr *) &serv_addr,serv_addr_len)) < 0){
            perror("send error");
            exit(EXIT_FAILURE);
        }

        memset(data_buff,0,DATA_BUFF_SIZE); //Remise à 0 du buffer de data
        pack++; //le prochain paquet aura un nouvel indice
    } while (env == BUFF_SIZE);

    fclose(fp); //Fermeture du fichier
    close(sock); //Fermeture du Socket
    return 0;
}
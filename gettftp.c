#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    FILE *fp; //pointeur de fichier

    int sock; //int de test 
    ssize_t env, rec; //int de longueur de message
    UNUSED(args);

    //définition du masque
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPV4
    hints.ai_socktype = SOCK_DGRAM; //Type de socket
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP; //protocole UDP

    //création de la commande PRQ
    int lencom = strlen(argv[2]);
    char *req = malloc(lencom+9*sizeof(char));
    req[0]=0;
    req[1]=1; //ID PRQ
    strcpy(&req[2],argv[2]); //nom du fichier
    req[lencom+2]=0;
    strcpy(&req[lencom+3],octet); //type
    req[lencom+8]=0;

    getaddrinfo(argv[1], PORT, &hints , &ai);//récupération des données serveur

    //Ouverture du socket
    if ((sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    //Envoi de la requête PRQ
    if ((env = sendto(sock,req, lencom+9, 0,ai->ai_addr,ai->ai_addrlen)) < 0){
        perror("send error");
        exit(EXIT_FAILURE);
    }

    //Ouverture (création) du fichier de récéption en mode write
    fp = fopen(argv[2], "w");

    do {
        serv_addr_len = sizeof(struct sockaddr_storage);

        //Réception d'un paquet de DATA
        if ((rec = recvfrom(sock, buff, BUFF_SIZE, 0,(struct sockaddr *) &serv_addr, &serv_addr_len))< 0){
            perror("recv error");
            exit(EXIT_FAILURE);
        }
        
        fwrite(buff+4,rec-4,1,fp); //Ecriture des datas recus dans le fichier (+4 pour retirer le header)
        buff[1] = 4; //Transformation du paquet DATA (ID 3) en ACK (ID 4)
        sendto(sock,buff,4,0,(struct sockaddr *) &serv_addr,serv_addr_len); //Envoi de l'ACK (seulement le header)
    } while (rec == BUFF_SIZE); //Sorti dès que le paquet reçu n'est pas plein

    fclose(fp); //Fermeture du fichier
    close(sock); //Fermeture du Socket
    return 0;
}
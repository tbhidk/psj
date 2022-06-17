#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 255
#define SA struct sockaddr

void func(int connfd, char * oFile)
{
    char buff[MAX];
    int n;
    FILE * fp;

    while (1) {
        fp = fopen(oFile, "a+");
        if (fp == NULL) {
            perror("Error opening file");
            return(-1);
        }
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        fputs(buff, fp);
        write(connfd, buff, sizeof(buff));
        printf("From client: %s\t To client : %s", buff, buff);
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
        fclose(fp);
    }
    close(connfd);
}
   
// Driver function
int main(int argc, char * argv[])
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    char * oFile;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    if (argc != 3)    // Test for correct number of arguments
    {
       fprintf(stderr, "Usage: %s <Echo Port> <File>\n", argv[0]);
       exit(1);
    }
       
    // assign IP
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    oFile = argv[2];

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
       
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
       
    while (1) {
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");
       
        func(connfd, oFile);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define BUF_SIZE 255
#define SA struct sockaddr
#define BAUDRATE B9600     
#define SERIALDEVICE "/dev/ttyS0"

void func(int sockfd)
{
    int serialfd, c, res;
    struct termios oldtio, newtio;
    char buff[BUF_SIZE];
    
    serialfd = open(SERIALDEVICE, O_RDWR | O_NOCTTY );
    if (serialfd < 0) {
        perror(SERIALDEVICE);
        exit(0);
    }
    
    tcgetattr(serialfd,&oldtio);
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    tcflush(serialfd, TCIFLUSH);
    tcsetattr(serialfd,TCSANOW,&newtio);

    while (1) {
        res = read(serialfd, buff, BUF_SIZE);
        buff[res] = 0;
        if (strlen(buff) > 1) {
            write(sockfd, buff, sizeof(buff));
            bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff));
            printf("From Server : %s", buff);
        }
    }
    tcsetattr(serialfd,TCSANOW,&oldtio);
}
   
int main(int argc, char * argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    if (argc != 3)    // Test for correct number of arguments
    {
       fprintf(stderr, "Usage: %s <Server IP> <Port>\n", argv[0]);
       exit(1);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else printf("Socket successfully created..\n");
    
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else printf("connected to the server..\n");
   
    // function for chat
    func(sockfd);
   
    // close the socket
    close(sockfd);
}

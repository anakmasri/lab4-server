#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>   //strlen
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>   //write

//func for chat between client & server
void func(int sock_desc){

    char buff[80];
    int n;

    //infinite loop for chat
    for (;;){
        bzero(buff, 80);

        //read the message from client and copy it in buffer
        read(sock_desc, buff, sizeof(buff));
        //print buffer which contains client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, 80);
        n = 0;

        //copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n');

        //send buffer to client
        write(sock_desc, buff, sizeof(buff));

        //if msg contains "Exit" then server exit and chat ended
        if (strncmp("exit", buff, 4) == 0){
            printf("Server Exit...\n");
            break;
        }
    }
}


int main(int argc, char *argv[]){

    int sock_desc, len, connfd;
    struct sockaddr_in server, client;

    //socket create and verification
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1){
        printf("Could not create socket\n");
    }
    else
        printf("Socket successfully created...\n");

    bzero(&server, sizeof(server));


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(8888);

    //binding newly created socket to given IP and verification
    if((bind(sock_desc, (struct sockaddr *)&server, sizeof(server))) != 0){
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    //server ready to listen and verification
    if((listen(sock_desc, 5)) != 0){
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

	len = sizeof(client);

    //accept the data packet from client and verification
    connfd = accept(sock_desc, (struct sockaddr *)&client, &len);
    if(connfd < 0){
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    //func for chatting between client and server
    func(connfd);

    //affter chatting close the socket
    close(sock_desc);
}

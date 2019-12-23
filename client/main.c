#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


const int MAX_PATH_LEN = 64;
const int COMMAND_SIZE = 6;
const int FULL_COMMAND_SIZE = MAX_PATH_LEN + COMMAND_SIZE ;
const int MAX_RESPONSE_LEN = 256;
const int MAX_REQUST_LEN = MAX_RESPONSE_LEN;

int main()
{
    int sock;
    struct sockaddr_in addr;
    char response[MAX_RESPONSE_LEN];
    char full_command[FULL_COMMAND_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    recv(sock, response, sizeof(response), 0);
    printf("%s\n", response);
    while(1) {
        fgets(full_command, FULL_COMMAND_SIZE, stdin);
        if(full_command[0] == '\n'){
            continue;
        }
        for(int i = 0; i < FULL_COMMAND_SIZE; i++){
            if(full_command[i] == '\n'){
                full_command[i] = '\0';
                break;
            }
        }

        send(sock, full_command, FULL_COMMAND_SIZE, 0);
        if(!strcmp(full_command, "exit")){
            break;
        }

        recv(sock, response, MAX_RESPONSE_LEN, 0);
        printf("%s\n", response);
    }
    close(sock);

    return 0;
}
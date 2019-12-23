#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "settings.h"
#include "commands/initial.h"
#include "commands/main.h"


int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    if (setsockopt (listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1) {
        perror("setsockopt");
        exit(3);
    }

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        char response[MAX_RESPONSE_LEN];

        if(get_fs_creation_status() == 1){
            load_file_system_structure();

            strcpy(response, "File system is ready!\n"
                    "Enter command:\0");
            send(sock, response, strlen(response) + 1, 0);

        }
        else{
            init_file_system();
            load_file_system_structure();

            strcpy(response, "File system created!\0");
            send(sock, response, strlen(response) + 1, 0);
        }

        char full_command[FULL_COMMAND_SIZE];
        while(1)
        {
            recv(sock, full_command, FULL_COMMAND_SIZE, 0);

            if(!strcmp(full_command, INIT)){
                if(get_fs_creation_status() == 1){
                    strcpy(response, "File system has already been created.\n"
                            "Want to re-create? (All data will be lost) [yes/no]:\0");
                    send(sock, response, strlen(response) + 1, 0);

                    char answer[4];
                    recv(sock, answer, 4, 0);
                    if(strcmp(answer, "yes\0")){ //no
                        continue;
                    }
                }
                printf( "Processing...\n");

                init_file_system();
                load_file_system_structure();

                strcpy(response, "File system created!\0");
                send(sock, response, strlen(response) + 1, 0);
            }
            else if(!strcmp(full_command, EXIT)){
                break;
            }
            else if(!strcmp(full_command, HELP)){
                sprintf(response, "%s\n"
                                "%s\n"
                                "%s <path>\n"
                                "%s <path>\n"
                                "%s <path>\n"
                                "%s <path>\n"
                                "%s <path>\n"
                                "%s <path>\n"
                                "%s <path>\0",
                        INIT, EXIT, MKDIR, RMDIR, TOUCH, RM, LS, CAT, ECHO);
                send(sock, response, strlen(response) + 1, 0);
            }
            else if(!strcmp(full_command, LS)){
                strcpy(response, get_ls_directory("\0"));
                send(sock, response, strlen(response) + 1, 0);
            }
            else {
                char command[COMMAND_SIZE];
                char path[MAX_PATH_LEN];

                command[0] = '\0';
                path[0] = '\0';

                get_command_and_path(full_command, command, path);
                if(!strcmp(command, MKDIR)){
                    printf( "Processing...\n");
                    if(create_directory(path) != 0){
                        strcpy(response, "Error\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                    else {
                        strcpy(response, "Directory created!\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                }
                else if(!strcmp(command, RMDIR)){
                    printf( "Processing...\n");
                    if(delete_directory(path) != 0){
                        strcpy(response, "Error\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                    else {
                        strcpy(response, "Directory deleted!\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                }
                else if(!strcmp(command, TOUCH)){
                    printf( "Processing...\n");
                    if(create_file(path) != 0){
                        strcpy(response, "Error\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                    else {
                        strcpy(response, "File created!\0");
                        send(sock, response, strlen(response) + 1, 0);
                    };
                }
                else if(!strcmp(command, RM)){
                    printf( "Processing...\n");
                    if(delete_file(path) != 0){
                        strcpy(response, "Error\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                    else {
                        strcpy(response, "File deleted!\0");
                        send(sock, response, strlen(response) + 1, 0);
                    };
                }
                else if(!strcmp(command, LS)){
                    strcpy(response, get_ls_directory(path));
                    send(sock, response, strlen(response) + 1, 0);
                }
                else if(!strcmp(command, CAT)){
                    strcpy(response, get_file(path));
                    send(sock, response, strlen(response) + 1, 0);
                }
                else if(!strcmp(command, ECHO)){
                    strcpy(response, "Enter text:\0");
                    send(sock, response, strlen(response) + 1, 0);

                    char buf[BLOCK_SIZE];
                    recv(sock, buf, BLOCK_SIZE, 0);

                    if(write_file(path, buf) != 0){
                        strcpy(response, "Error\0");
                        send(sock, response, strlen(response) + 1, 0);
                    } else {
                        strcpy(response, "Text saved\0");
                        send(sock, response, strlen(response) + 1, 0);
                    }
                }
                else{
                    strcpy(response, "Unknown command\0");
                    send(sock, response, strlen(response) + 1, 0);
                }
            }
        }
    }

    close(sock);

    return 0;
}
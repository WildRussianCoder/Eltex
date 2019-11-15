#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include "init.h"
#include <string.h>
#include <unistd.h>

struct sockaddr server_address = 
    {AF_INET, PORT, ADDRESS, RESERV};

int main(int argc, char** argv){
    int server = socket(AF_INET, SOCK_STREAM, 0);
    char file_count = argc - 1;

    connect(server, &server_address, sizeof(server_address));
    printf("Соединение установлено\n");

    printf("Передача %d имен\n", argc - 1);
    send(server, &file_count, 1, 0);
    for(int i = 0; i < file_count; i++)
        send(server, argv[i + 1], strlen(argv[i + 1]) + 1, 0);
    printf("Передача завершена\n");

    for(int i = 0; i < file_count; i++){
        char name[MAX_LEN];
        long sum;
        recv(server, name, MAX_LEN, 0);
        recv(server, &sum, sizeof(long), 0);
        printf("Контрольная сумма файла %s: %ld\n", name, sum);
    }
    close(server);
    return 0;
}
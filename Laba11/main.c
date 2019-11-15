#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "init.h"

pthread_t tids[MAX_FILES];

struct sockaddr server_address =
    {AF_INET, PORT, ADDRESS, RESERV};

char files[MAX_FILES][MAX_LEN];
char file_count;

void* check_sum(void* arg){
    long sum = 0;
    char byte = 0;

    int fd = open(arg, O_RDONLY);
    while(read(fd, &byte, 1) != 0) sum += byte;
    close(fd);
    return (void*)sum;
}

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock, &server_address, sizeof(struct sockaddr));
    listen(sock, 10);

    while(1){
        int client_sock = accept(sock, NULL, NULL);
        printf("Соединение установлено!\n");

        recv(client_sock, &file_count, 1, 0);

        printf("Будет принято %d имен файлов\n", file_count);

        for(char i = 0; i < file_count; i++)
            if(i < MAX_FILES)
                recv(client_sock, files[i], MAX_LEN, 0);
        printf("Принято %d имен\n", file_count);

        printf("Запуск %d потоков для обработки файлов\n", file_count);
        for(int i = 0; i < file_count; i++)
            if(i < MAX_FILES) 
                pthread_create(&tids[i], NULL, check_sum, files[i]);
        printf("Потоки запущены\n");

        for(int i = 0; i < file_count; i++)
            if(i < MAX_FILES){
                long sum;
                pthread_join(tids[i], (void*)&sum);
                printf("Отправка данных клиенту\n");
                send(client_sock, files[i], MAX_LEN, 0);
                send(client_sock, &sum, sizeof(sum), 0);
            }        
        printf("Отправка завершена\n");
        printf("Закрытие соединения с клиентом\n");
        close(client_sock);
    }
}
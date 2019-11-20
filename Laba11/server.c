#include <stdio.h>
#include <pthread.h>
#include "init.h"
#include <fcntl.h>

char name[NAME_LEN];
pthread_t tids[MAX_FILES];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char to_process[MAX_FILES][NAME_LEN];
int file_cnt = 0;
struct sockaddr_in host;
int client_fd;


void* thread(void* arg){
    char* n = arg;
    char byte;
    long sum = 0;

    int fd = open(n, O_RDONLY);
    if(fd < 0)
        return 0;
    
    while( read(fd, &byte, 1) > 0 ) sum += byte;

    char buf[NAME_LEN];
    sprintf(buf, "%s: %ld\n", n, sum);

    pthread_mutex_lock(&mutex);
    write(client_fd, buf, strlen(buf) + 1);
    pthread_mutex_unlock(&mutex);

    return 0;
}

void process_client(int argc, char** argv){
    char* msg = "Перечислите имена файлов для вычисления контрольных суммм(разделять через Entet):\n";
    
    write(client_fd, msg, strlen(msg) + 1);

    for(int i = 1; i < argc; i++)
        write(client_fd, argv[i], strlen(argv[i]) + 1);
    
    write(client_fd, EOT, strlen(EOT) + 1);

    for(int i = 0; i < argc - 1; i++){
        if( read(client_fd, &name, NAME_LEN) < 0){
            perror("Error read()");
            exit(-1);
        }

        if( strcmp(name, EOT) == 0 ) break;

        strcpy(to_process[i], name);
        file_cnt++;
    }

    for(int i = 0; i < file_cnt; i++)
        pthread_create(&tids[i], NULL, thread, to_process[i]);

    

}

int main(int argc, char** argv){
    if( argc < 2){
        printf("Серверу необходимо передать список файлов" 
                "доступных клиентам для запроса контрольных сумм\n");
        return -1;
    }

    host.sin_family = AF_INET;
    host.sin_port = htons(PORT);

    int host_fd = socket(AF_INET, SOCK_STREAM, 0);
    if( host_fd < 0 ){
        perror("Error socket()"); 
        return -1;
    }

    if( bind(host_fd, (struct sockaddr*)&host, sizeof(host)) < 0 ){
        perror("Error bind()");
        return -1;
    }

    if( listen(host_fd, Q) < 0 ){
        perror("Error listen()");
        return -1;
    }

    while(1){
        client_fd = accept(host_fd, NULL, NULL);
        if(client_fd < 0){
            perror("Error accept()");
            return -1;
        }

        pid_t pid = fork();
        if(pid == 0) {
            close(host_fd);
            process_client(argc, argv);
            return 0;
        }
        close(client_fd);
    }
}
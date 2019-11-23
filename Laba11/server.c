#include "init.h"


DIR *server_dir;
struct dirent *server_dir_item;
struct sockaddr_in server_addr;
socklen_t server_addr_len = sizeof(server_addr);
int server_sock;

int client_sock;

char files[MAX_FILES][NAME_LEN];
int file_cnt;

void process_client()
{  
    char name[NAME_LEN];

    if( read(client_sock, name, NAME_LEN) < 0 )
    {
        perror("Error read() *name*");
        exit(-1);
    }

    if( name[0] == 0)
    {
        if( write(client_sock, &file_cnt, sizeof(file_cnt)) < 0 )
        {
            perror("Error write() *cnt*");
            exit(-1);
        }
        
        for(int i = 0; i < file_cnt; i++)
        {
            if( write(client_sock, &i, sizeof(i)) < 0)
            {
                perror("Error write() *i*");
                exit(-1);
            }

            if( write(client_sock, files[i], NAME_LEN) < 0)
            {
                perror("Error write() *name*");
                exit(-1);
            }
        }
        exit(0);
    }
    
    long cs = 0;
    char byte = 0;

    int fd = open(name, O_RDONLY);
    if( fd < 0 )
    {
        write(client_sock, &cs, sizeof(cs));
        perror("Error open()");
        exit(-1);
    }

    while( read(fd, &byte, sizeof(byte)) > 0 ) cs += byte;

    if( write(client_sock, &cs, sizeof(cs)) < 0 )
    {
        perror("Error write()");
        exit(-1);
    }

    exit(0);
}

int main(int argc, char** argv) 
{
    memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_dir = opendir(".");
    if(server_dir == NULL)
    {
        perror("Error opendir()");
        exit(-1);
    }

    for(int i = 0; i < MAX_FILES; i++)
    {
        server_dir_item = readdir(server_dir);

        if(server_dir_item == NULL) break;
        
        if(server_dir_item->d_type == 8)
            strcpy(files[file_cnt++], server_dir_item->d_name);
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0)
    {
        perror("Error socket()");
        exit(-1);
    }

    if( bind(server_sock, (struct sockaddr*)&server_addr, server_addr_len) < 0 )
    {
        perror("Error bind()");
        exit(-1);
    }

    if( listen(server_sock, Q) < 0 )
    {
        perror("Error listen()");
        exit(-1);
    }

    while(1)
    {
        client_sock = accept(server_sock, NULL, NULL);
        if(client_sock < 0)
        {
            perror("Error accept()");
            exit(-1);
        }

        pid_t pid = fork();
        if(pid < 0)
        {
            perror("Error fork()");
            exit(-1);
        }
        else if(pid == 0) 
        {
            process_client();
        }
        
        if( close(client_sock) < 0 )
        {
            perror("Error close()");
            exit(-1);
        }
    }
}
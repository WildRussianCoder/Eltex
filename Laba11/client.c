#include "init.h"

struct sockaddr_in server_addr;
socklen_t server_addr_len = sizeof(server_addr);
int server_sock;

int file_cnt;
long cs;

int main(int argc, char** argv)
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(argc == 2 || argc == 3)
    {
        if ( inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) < 0 )
        {
            perror("Error inet_pton()");
            exit(-1);
        }

        server_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(server_sock < 0)
        {
            perror("Error socket()");
            exit(-1);
        }

        if ( connect(server_sock, (struct sockaddr*)&server_addr, server_addr_len) < 0 )
        {
            perror("Error connect()");
            exit(-1);
        }

        if(argc == 2)
        {
            if (write(server_sock, &cs, sizeof(cs)) < 0)
            {
                perror("Error write() *z-name*");
                exit(-1);
            }

            if( read(server_sock, &file_cnt, sizeof(file_cnt)) < 0 )
            {
                perror("Error read() *file_cnt*");
                exit(-1);
            }

            for(int i = 0; i < file_cnt; i++)
            {
                char name[NAME_LEN];
                int num;

                if( read(server_sock, &num, sizeof(num)) < 0 )
                {
                    perror("Error read() *num*");
                    exit(-1);
                }

                if( read(server_sock, name, NAME_LEN) < 0 )
                {
                    perror("Error read() *name*");
                    exit(-1);
                }

                printf("%d. %s\n", num, name);
            }       
        }
        else
        {
            if(write(server_sock, argv[2], strlen(argv[2]) + 1) < 0)
            {
                perror("Error write() *name*");
                exit(-1);
            }

            if(read(server_sock, &cs, sizeof(cs)) < 0)
            {
                perror("Error read()");
                exit(-1);
            }

            printf("Контрольная сумма %s: %ld\n", argv[2], cs);
        }
        
        if(close(server_sock) < 0)
        {
            perror("Error close()");
            exit(-1);
        }
    }
    else 
    {
        printf("Используйте:\n");
        printf("\tclient <IPv4> - чтобы увидеть список файлов\n");
        printf("\tclient <IPv4> <name_of_file> - чтобы получить контрольную сумму\n");
        exit(-1);        
    }   
}
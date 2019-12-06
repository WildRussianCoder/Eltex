#include "init.h"

/* sockaddr для прослушивающего TCP-сокета */
struct sockaddr_in addr;

/* Структура с атрибутами очереди сообщений */
struct mq_attr qattr;

/* Потоковая функция получающая от клиента сообщение и добавляющая это сообщение в очередь сообщений */
void* recv_from_client ( void* arg )
{

}

int main ()
{
    /* Устанавливаем параметры sockaddr */
    addr.sin_family = AF_INET;
    addr.sin_port = htons ( LISTENER_TCP_PORT );
    addr.sin_addr.s_addr = htonl ( INADDR_ANY );

    /* Открываем созданную очередь сообщений */
    mqd_t q = mq_open(Q_NAME, O_WRONLY);
    if ( q < 0 )
    {
        perror ( "Ошибка открытия очереди сообщений в listener" );
        exit ( -1 );
    }

    /* Создаем прослушивающий сокет */
    int sock = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( sock < 0 )
    {
        perror ( "Ошибка создания сокета в listener" );
        exit ( -1 );
    }

    /* Связываем прослушивающий сокет с портом */
    if ( bind ( sock, ( struct sockaddr* ) &addr, sizeof ( addr ) ) < 0 )
    {
        perror ( "Ошибка связывания сокета в listener" );
        exit ( -1 );
    }

    /* Устанавливаем очередь соединений */
    if ( listen ( sock, CLIENT_Q ) < 0 )
    {
        perror ( "Ошибка создания очереди клиентов в listener" );
        exit ( -1 );
    }

    /* В бесконечном цикле принимаем подключения клиентов и порождаем для них новых поток */
    while ( 1 )
    {
        /* Принимаем подключение клиента */
        int client_sock = accept ( sock, NULL, NULL );

        /* Получаем атрибуты очереди сообщений, чтобы проверить кол-во сообщений в очереди */
        mq_getattr ( q, &qattr );

        /* Если очередь пуста, то отбрасываем клиента, иначе обрабатываем его в потоке */
        if ( qattr.mq_curmsgs == 0 )
            close ( client_sock );
        else 
        {
            if ( pthread_create ( NULL, NULL, recv_from_client, ( long ) client_sock ) < 0 )
            {
                perror ( "Ошибка создания потока в listener" );
                exit ( - 1 );
            }
        }
    }
}
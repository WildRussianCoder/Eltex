#include "init.h"

/* Параметры очереди сообщений */
struct mq_attr qattr;

int main ()
{
    /* Устанавливаем параметры очереди сообщений */
    qattr.mq_maxmsg = MAX_MSG;
    qattr.mq_msgsize = MAX_MSG_SIZE;

    /* Если очередь сообщений уже создана, то удаляем её */
    mq_unlink ( Q_NAME );

    /* Создаем очередь сообщений */
    mqd_t q = mq_open ( Q_NAME, O_CREAT | O_EXCL, 0666, &qattr );
    if ( q == -1 )
    {
        perror ( "Ошибка создания очереди сообщений" );
        exit(-1);
    }

    /* Порождаем send-сервер */
    pid_t p = fork ();
    if ( p < 0 ) 
    {
        perror ( "Ошибка создания send-сервера" );
        mq_close ( q );
        mq_unlink ( Q_NAME );
        exit ( -1 );
    }
    else if ( p == 0 ) 
        execl ( "./sender.out", "sender", NULL );

    /* Порождаем listen-сервер */
    p = fork ();
    if ( p < 0 )
    {
        perror ( "Ошибка создания listen-сервера" );
        mq_close ( q );
        mq_unlink ( Q_NAME );
        exit ( -1 );
    }
    else if ( p == 0 ) 
        execl ( "./listener.out", "listener", NULL );

    /* Ждём ввода точки, чтобы завершить работу сервера */
    while ( 1 )
    {
        char c = getchar ();
        if ( c == '.' )
        {
            mq_close ( q );
            mq_unlink ( Q_NAME );
            kill ( 0, SIGKILL );
        }
    }    
}
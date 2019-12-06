#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define Q_NAME "/NameOfMQ"
#define MAX_MSG 10
#define MAX_MSG_SIZE 4096
#define CLIENT_Q 10
#define LISTENER_TCP_PORT 60000
#define SENDER_TCP_PORT 60001
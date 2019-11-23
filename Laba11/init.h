#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>
#include <arpa/inet.h>

#define PORT 60000
#define Q 100
#define NAME_LEN 256
#define EOT "."
#define MAX_FILES 100
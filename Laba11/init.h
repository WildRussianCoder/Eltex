#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define PORT 60000
#define Q 100
#define NAME_LEN 1024
#define EOT "."
#define MAX_FILES 100
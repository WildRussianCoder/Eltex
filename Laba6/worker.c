#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

char buf[7];

int main(int argc, char** argv){
    int t = atoi(argv[1]);
    int fd = open("./tmp", O_RDWR | O_ASYNC);
    printf("Рабочий %d создан\n", getpid());

    while(1){
        sleep(t);
        flock(fd, LOCK_EX);
        read(fd, buf, sizeof(buf));
        lseek(fd, 0, SEEK_SET);
        int g = atoi(buf);
        if(g <= 0) break;
        g -= 10;
        sprintf(buf, "%d", g);
        write(fd, buf, sizeof(buf));
        lseek(fd, 0, SEEK_SET);
        flock(fd, LOCK_UN);
    }

    printf("Рабочий %d умер\n", getpid());
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}
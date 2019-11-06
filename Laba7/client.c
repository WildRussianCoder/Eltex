#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/file.h>

int main(int argc, char** argv){
    srand(getpid());
    int t = 1 + rand() % 3;
    int portion = 2 + rand() % 8;

    int fifo_fd = open(argv[1], O_WRONLY | O_NONBLOCK);
    int status_fd = open(argv[2], O_RDONLY);

    printf("Я пчела %d, Я буду носить по %d меда за %d секунд\n", getpid(), portion, t);

    while(1){
        sleep(t);

        flock(status_fd, LOCK_EX);
        char status;
        read(status_fd, &status, 1);
        lseek(status_fd, 0, SEEK_SET);
        flock(status_fd, LOCK_UN);

        if(status == 0) break;

        write(fifo_fd, &portion, sizeof(int));
        
        printf("Я пчела %d, Я принесла мёд\n", getpid());
    }
    
    printf("Я пчела %d, Я умерла\n", getpid());
    close(fifo_fd);
    close(status_fd);
    return 0;
}
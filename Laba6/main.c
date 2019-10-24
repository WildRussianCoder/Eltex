#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>

#define START_GOLD 100
#define MAX_POSITIONS_IN_NUMBER 6
#define PERMISSIONS 0755


char buf[MAX_POSITIONS_IN_NUMBER + 1];

int main(){
    srand(time(NULL));
    int fd = open("./tmp", O_CREAT | O_RDWR | O_ASYNC | O_TRUNC, PERMISSIONS);

    
    sprintf(buf, "%d", START_GOLD);
    printf("Начальное количество золота: %s\n", buf);

    write(fd, buf, sizeof(buf));
    lseek(fd, 0, SEEK_SET);

    int workers;
    printf("Укажите начальное кол-во рабочих: ");
    scanf("%d", &workers);

    int* t = calloc(workers, sizeof(int));
    for(int i = 0; i < workers; i++){
        t[i] = 1 + rand() % 4;
        for(int i = 0; i < 10000; i++);
    }

    for(int i = 0; i < workers; i++){
        char arg[3];
        sprintf(arg, "%d", t[i]);
        if(fork() == 0) execl("./worker", "worker", arg, NULL);
    }

    while(1){
        sleep(1);
        flock(fd, LOCK_EX);
        read(fd, buf, sizeof(buf));
        lseek(fd, 0, SEEK_SET);
        int g = atoi(buf);
        if(g <= 0) break;
        printf("В шахте осталось: %s\n", buf);
        flock(fd, LOCK_UN);
    }

    printf("Золотой рудник иссяк\n");

    flock(fd, LOCK_UN);
    free(t);
    close(fd);
}
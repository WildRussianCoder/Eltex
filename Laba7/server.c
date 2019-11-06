#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/file.h>

#define FIFO_NAME "fifo"
#define STATUS_NAME "status"

int total, cnt, ls, h, fifo_fd, status_fd;
pid_t* pids;
pid_t p;
char status = 1;

int main(){
    printf("Укажите кол-во пчёл: ");
    scanf("%d", &cnt);
    pids = malloc(cnt * sizeof(pid_t));

    printf("Укажите время жизни Вини-Пуха: ");
    scanf("%d", &ls);

    printf("Укажите голод Вини-Пуха: ");
    scanf("%d", &h);

    mkfifo(FIFO_NAME, 0666);
    fifo_fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);

    status_fd = creat(STATUS_NAME, 0666);
    flock(status_fd, LOCK_EX);
    write(status_fd, &status, 1);
    lseek(status_fd, 0, SEEK_SET);
    flock(status_fd, LOCK_UN);

    for(int i = 0; i < cnt; i++){
        p = fork();
        if(p == 0) execl("./client", "client", FIFO_NAME, STATUS_NAME, NULL);
        pids[i] = p;
    }


    while(1){
        printf("Медведь заснул\n");
        sleep(ls);
        printf("Медведь проснулся\n");

        while(total < h){
            int hon = 0;
            if(read(fifo_fd, &hon, sizeof(int)) == -1) goto quit;
            total += hon;
        }
        printf("Медведь собрал мед\n");

        total -= h;
        printf("Медведь покушал\n");
    }

    quit:
    printf("Медведю не хватило еды, и он скончался\n");

    status = 0;

    flock(status_fd, LOCK_EX);
    write(status_fd, &status, 1);
    flock(status_fd, LOCK_UN);

    for(int i = 0; i < cnt; i++) waitpid(pids[i], NULL, 0);

    close(status_fd);
    close(fifo_fd);

    unlink(FIFO_NAME);
    unlink(STATUS_NAME);
    
    free(pids);
    return 0;
}
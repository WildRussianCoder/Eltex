#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define KEY 0x200
#define MSGSZ 0x03

int cnt, width, height, msg_id;
pid_t* pids;
struct msqid_ds q;

struct msgs{
    long mtype;
    char mtext[MSGSZ];
}message;

void ball(int n, char w, char h){
    srand(getpid());

    char x, y;

    message.mtype = 1;
    message.mtext[0] = n;

    printf("Шарик %d создан!\n", n);

    while(1){
        sleep(1);

        x = rand() % (w + 10);
        y = rand() % (h + 10);

        message.mtext[1] = x;
        message.mtext[2] = y;

        if(x >= w || y >= h) break;

        msgsnd(msg_id, &message, MSGSZ, IPC_NOWAIT);
    }

    message.mtext[0] = -1;
    msgsnd(msg_id, &message, MSGSZ, IPC_NOWAIT);

    printf("%d вылетел за пределы области: (%d, %d)\n", n, x, y);
    
    exit(0);
}

int main(){
    printf("Укажите ширину поля: ");
    scanf("%d", &width);

    printf("Укажите высоту поля: ");
    scanf("%d", &height);

    printf("Укажите кол-во шариков: ");
    scanf("%d", &cnt);
    
    pids = malloc(sizeof(pid_t) * cnt);

    msg_id = msgget(KEY, IPC_CREAT | IPC_EXCL | 0666);
    if(msg_id < 0){
        perror("msgget");
        exit(-1);
    }

    for(int i = 0; i < cnt; i++){
        pid_t p = fork();
        if(p == 0) ball(i, width, height);
        pids[i] = p;
    }

    int dead = 0;
    while(1){
        msgrcv(msg_id, &message, MSGSZ, 1, 0);

        if(message.mtext[0] == -1) dead++;
        else printf("%d: (%d, %d)\n", message.mtext[0], message.mtext[1], message.mtext[2]);

        if(dead >= cnt) break;
    }

    msgctl(msg_id, IPC_RMID, &q);
    free(pids);
    return 0;
}
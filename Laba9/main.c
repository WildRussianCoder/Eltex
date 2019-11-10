#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEM_KEY 0x300
#define SHM_KEY 0x400

int main(int argc, char** argv){

    if(argc != 2){
        printf("Используйте: ./l9 <кол-во команд>\n");
        exit(-1);
    }

    int cnt = atoi(argv[1]);
    pid_t* pids = malloc(sizeof(pid_t) * cnt);

    struct sembuf sem_init = {0, 1, SEM_UNDO};
    int sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    semop(sem_id, &sem_init, 1);

    int shm_id = shmget(SHM_KEY, cnt + 1, 0666 | IPC_CREAT);
    void* arr = shmat(shm_id, NULL, 0);
    *((char*)arr) = cnt;

    for(int i = 1; i <= cnt; i++){
        char num[10];
        sprintf(num, "%d", i);
        
        pid_t p = fork();

        if(p == 0) execl("./team", "team", argv[1], num, NULL);

        pids[i] = p;
    }

    for(int i = 0; i < cnt; i++) waitpid(pids[i], NULL, 0);
    free(pids);
    semctl(sem_id, 1, IPC_RMID);
    shmctl(shm_id, IPC_RMID, 0);
    return 0;
}
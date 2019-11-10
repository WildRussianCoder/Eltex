#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/shm.h>

#define SEM_KEY 0x300
#define SHM_KEY 0x400
#define MAX_POINTS 10
#define START_POINTS 20

struct sembuf sem_lock = {0, -1, SEM_UNDO};
struct sembuf sem_unlock = {0, 1, SEM_UNDO};

int main(int argc, char** argv){
    srand(getpid());
    /* Getting args */
    int cnt = atoi(argv[1]);
    int num = atoi(argv[2]);

    /* Getting semaphore (Sys V) */
    int sem_id = semget(SEM_KEY, 1, 0666);
    if(sem_id < 0){
        perror("semget");
        exit(-1);
    }

    /* Getting shared memory (Sys V) */
    int shm_id = shmget(SHM_KEY, cnt + 1, 0666);
    if(shm_id < 0){
        perror("shmget");
        exit(-1);
    }
    char* teams = shmat(shm_id, NULL, 0);

    /* Team initialization */
    teams[num] = START_POINTS;

    /* Infinity fighting loop */
    while(1){
        semop(sem_id, &sem_lock, 1);

        if(teams[num] == 0) {
            printf("Команда %d уничтожена\n", num);
            break;
        }

        if(teams[0] == 1) {
            printf("Команда %d победила\n", num);
            break;
        }

        int team_dmg = rand() % MAX_POINTS;
        int team_heal = MAX_POINTS - team_dmg;

        int target;
        do{
            target = 1 + rand() % cnt;
        }while(target == num || teams[target] == 0);
        
        if(teams[target] < team_dmg) teams[target] = 0;
        else teams[target] -= team_dmg;
        printf("Команда %d атаковала команду %d: %d\n", num, target, team_dmg);

        if(255 - teams[num] < team_heal) teams[num] = 255;
        else teams[num] += team_heal;
        printf("В команде %d пополнение: %d\n", num, team_heal);

        usleep(100000);

        semop(sem_id, &sem_unlock, 1);
    }

    teams[0]--;
    semop(sem_id, &sem_unlock, 1);
    exit(0);
}
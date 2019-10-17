#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int gold = 1000;
int workers;

int main(int argc, char** argv){
    printf("Укажите кол-во рабочих: ");
    scanf("%d", &workers);

    while(gold > 0){
        printf("Текущее количество золота: %d\n", gold);
        if(workers > 0){
            if(fork() == 0) execl("./unit", "unit", NULL);
            workers--;
            gold -= 10;
        }
        else{
            wait(NULL);
            if(fork() == 0) execl("./unit", "unit", NULL);
            gold -= 10;
        }
    }

    printf("Золотой рудник истощился!\n");

    return 0;
}
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    sleep(1 + rand() % 5);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LEN 100
#define MAX_DATA 10

typedef struct _data{
    int len;
    float M;
    int* arr;
} data;

void* process_data(void* arg){
    data* d = (data*)arg;
    
    int collection[MAX_DATA];
    for(int i = 0; i < MAX_DATA; i++) collection[i] = 0;

    float ver[MAX_DATA];
    for(int i = 0; i < MAX_DATA; i++) ver[i] = 0;

    for(int i = 0; i < d->len; i++) collection[d->arr[i]]++;

    for(int i = 0; i < MAX_DATA; i++)
            ver[i] = (float)collection[i] / (float)d->len;

    for(int i = 0; i < d->len; i++){
        d->M += d->arr[i] * ver[d->arr[i]];
    }

}

void data_rnd(data* d){
    int len = 1 + rand() % (MAX_LEN - 1);
    d->len = len;
    d->arr = calloc(len, sizeof(int));
    for(int i = 0; i < len; i++) d->arr[i] = rand() % MAX_DATA;
    d->M = 0.0;
}

int main(){    
    srand(getpid());

    int cnt;
    printf("Укажите кол-во масивов данных: ");
    scanf("%d", &cnt);

    data* all_data = malloc(sizeof(data) * cnt);
    for(int i = 0; i < cnt; i++) data_rnd(&all_data[i]);

    pthread_t* tids = malloc(sizeof(pthread_t) * cnt);

    for(int i = 0; i < cnt; i++)
        pthread_create(&tids[i], NULL, process_data, &all_data[i]);

    for(int i = 0; i < cnt; i++) {
        pthread_join(tids[i], NULL);
        printf("M%d = %f\n", i, all_data[i].M);
        free(all_data[i].arr);
    }

    free(all_data);
    free(tids);
    return 0;
}
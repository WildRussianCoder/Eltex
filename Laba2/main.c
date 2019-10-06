#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int inp_str(char** string, int maxlen);
void out_str(char* string, int length, int number);
void rev_abc_sort(char*** arr, int** arr_of_len, int len, int* count_of_ops, int* len_of_first_word);

int main(int argc, char** argv){
    int count = 0;
    printf("Укажите кол-во строк:\n");
    scanf("%d", &count);
    getchar();

    char** strings = (char**)calloc(count, sizeof(char*));
    int* len_of_strings = (int*)calloc(count, sizeof(int));

    for(int i = 0; i < count; i++){
        len_of_strings[i] = inp_str(&strings[i], BUF_SIZE);
    }

    printf("\nИсходный набор слов:\n");
    for(int i = 0; i < count; i++){
        out_str(strings[i], len_of_strings[i], i);
    }

    int ops;
    int first_len;
    rev_abc_sort(&strings, &len_of_strings, count, &ops, &first_len);

    printf("\nСортированный набор:\n");
    for(int i = 0; i < count; i++){
        out_str(strings[i], len_of_strings[i], i);
    }

    free(len_of_strings);
    free(strings);
}

void rev_abc_sort(char*** arr, int** arr_of_len, int len, int* count_of_ops, int* len_of_first_word){

}

int inp_str(char** string, int maxlen){
    char buf[maxlen];
    /* scanf("%s", buf);
    getchar(); */
    fgets(buf, maxlen, stdin);

    int size = 0;
    for(; buf[size] != 0; size++);

    char* ds = (char*)calloc(size, sizeof(char));
    for(int i = 0; i < size - 1; i++){
        ds[i] = buf[i];
    }
    *string = ds;

    return size;
}

void out_str(char* string, int length, int number){
    printf("%d: %d| %s\n", number, length, string);
}
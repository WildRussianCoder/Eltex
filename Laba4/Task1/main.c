#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* name_of_input_file;
char name_of_output_file[256];

int main(int argc, char** argv){

    if(argc != 3){
        printf("Неверное количество аргументов\n");
        return 1;
    }

    char symbol = argv[2][0];
    name_of_input_file = argv[1];
    for(int i = 0; name_of_input_file[i] != '.'; i++) name_of_output_file[i] = name_of_input_file[i];
    strcat(name_of_output_file, ".out");

    printf("Имя входного файла: %s\n", name_of_input_file);
    printf("Имя выходного файла: %s\n", name_of_output_file);
    printf("Символ: %c\n", symbol);

    FILE* input_file = fopen(name_of_input_file, "r");
    FILE* output_file = fopen(name_of_output_file, "w");

    if(input_file == NULL){
        printf("Ошибка открытия входного файла\n");
        return 1;
    }
    else if(output_file == NULL){
        printf("Ошибка открытия выходного файла\n");
        return 1;
    }

    while(1){
        char* buf = (char*)calloc(1024, sizeof(char));
        if(fgets(buf, 256, input_file) == NULL) break;

        if(buf[0] == symbol) fputs(buf, output_file);
        free(buf);
    }

    printf("Работа завершена\n");
    return 0;
}
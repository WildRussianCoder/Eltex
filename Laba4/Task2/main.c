#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* name_of_intput_file;
char name_of_output_file[256];
char symbol;
char buf[2];

int main(int argc, char** argv){

    if(argc != 3){
        printf("Неверное количество аргументов\n");
        return 1;
    }

    name_of_intput_file = argv[1];
    for(int i = 0; name_of_intput_file[i] != '.'; i++) name_of_output_file[i] = name_of_intput_file[i];
    strcat(name_of_output_file, ".out");
    symbol = argv[2][0];

    printf("Имя входного файла: %s\n", name_of_intput_file);
    printf("Имя выходного файла: %s\n", name_of_output_file);
    printf("Символ: %c\n", symbol);

    FILE* input_file = fopen(name_of_intput_file, "rb");
    FILE* output_file = fopen(name_of_output_file, "wb");

    if(input_file == NULL){
        printf("Ошибка открытия входного файла\n");
        return 1;
    }
    else if(output_file == NULL){
        printf("Ошибка открытия выходного файла\n");
        return 1;
    }

    while(!feof(input_file)){
        buf[0] = fgetc(input_file);
        
        if(strspn(buf, "!@#$%%^&*()_+`=-,./';][\\{}|:\"<>?") == 1) buf[0] = symbol;

        fputc(buf[0], output_file);
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вариант 4
// Все должники сверху, т.е. сортировка по убыванию долга

struct bomj{
    char* surname;
    char* birth_day;
    int need_to_pay;
};

void setSurname(struct bomj* b){
    printf("Введите фамилию: ");
    char buf[256];
    scanf("%s", buf);
    char* sur = (char*)calloc(strlen(buf) + 1, sizeof(char));
    strcpy(sur, buf);
    b->surname = sur;
}

void setBirthDay(struct bomj* b){
    printf("Введите дату рождения: ");
    char buf[256];
    scanf("%s", buf);
    char* bd = (char*)calloc(strlen(buf) + 1, sizeof(char));
    strcpy(bd, buf);
    b->birth_day = bd;
}

void setNeeded(struct bomj* b){
    printf("Введите сумму долга: ");
    int buf = 0;
    scanf("%d", &buf);
    b->need_to_pay = buf;
}

void bomjInit(struct bomj* b){
    setSurname(b);
    setBirthDay(b);
    setNeeded(b);
}
void bomjDelete(struct bomj* b){
    free(b->surname);
    free(b->birth_day);
}

void showBomjTable(struct bomj* b, int size){
    printf("\n=================================================\n");
    for(int i = 0; i < size; i++){
        printf("| %03d | %15s | %10s | %8d |\n", i, b[i].surname, b[i].birth_day, b[i].need_to_pay);
    }
    printf("=================================================\n");
}

void bomjSort(struct bomj* b, int size){
    for(int i = size - 1; i >= 0; i--){
        for(int j = 0; j < i; j++){
            if(b[j].need_to_pay < b[j + 1].need_to_pay){
                int tmp = b[j].need_to_pay;
                b[j].need_to_pay = b[j + 1].need_to_pay;
                b[j + 1].need_to_pay = tmp;

                char* tmp_sur = b[j].surname;
                b[j].surname = b[j + 1].surname;
                b[j + 1].surname = tmp_sur;

                char* tmp_bd = b[j].birth_day;
                b[j].birth_day = b[j + 1].birth_day;
                b[j + 1].birth_day = tmp_bd;
            }
        }
    }
}

int main(){
    int count = 0;
    printf("Укажите кол-во должников: ");
    scanf("%d", &count);

    struct bomj* bomji = (struct bomj*)malloc(count * sizeof(struct bomj));

    for(int i = 0; i < count; i++){
        bomjInit(&bomji[i]);
    }

    bomjSort(bomji, count);
    showBomjTable(bomji, count);

    for(int i = 0; i < count; i++){
        bomjDelete(&bomji[i]);
    }
    free(bomji);

    return 0;
}
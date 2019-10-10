#include <stdio.h>
#include <dlfcn.h>

int main(){
    int a, b;
    printf("Введите делимое: ");
    scanf("%d", &a);

    printf("Введите делитель(не ноль): ");
    scanf("%d", &b);

    if(b == 0){
        printf("Результат деления на 0 неопределен\n");
        return 1;
    }

    void* dl_handle = dlopen("mylib.so", RTLD_LAZY);
    if(dl_handle == NULL){
        printf ("Ошибка загрузки динамической библиотеки\n");
        return 1;
    }

    int (*div_foo)(int, int) = dlsym(dl_handle, "Div");
    int (*mod_foo)(int, int) = dlsym(dl_handle, "Mod");
    if(div_foo == NULL || mod_foo == NULL){
        printf ("Ошибка загрузки одной из функций динамической библиотеки\n");
        return 1;
    }
    
    printf("Частное от деления: %d\n", (*div_foo)(a,b));
    printf("Остаток от деления: %d\n", (*mod_foo)(a, b));
    
    dlclose(dl_handle);
}
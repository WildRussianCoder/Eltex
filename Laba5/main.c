#include <stdio.h>
#include <dlfcn.h>

#ifdef STATIC
extern int Mod(int a, int b);
extern int Div(int a, int b);
#endif

#ifdef DYNOSTATIC
extern int Mod(int a, int b);
extern int Div(int a, int b);
#endif

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

    #ifdef DYNAMIC
    void* dl_handle = dlopen("libmylib.so", RTLD_LAZY);
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
    #endif

    #ifdef STATIC
    printf("Частно от деления: %d\n", Div(a, b));
    printf("Остаток от деления: %d\n", Mod(a, b));
    #endif

    #ifdef DYNOSTATIC
    printf("Частно от деления: %d\n", Div(a, b));
    printf("Остаток от деления: %d\n", Mod(a, b));
    #endif
}
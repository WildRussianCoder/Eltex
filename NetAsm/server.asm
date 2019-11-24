format ELF64

public _start

AF_INET equ 2
SOCK_STREAM equ 1
Q_SIZE equ 10
BUF_SIZE equ 1024

SYS_SOCKET equ 41
SYS_BIND equ 49
SYS_EXIT equ 60
SYS_LISTEN equ 50
SYS_ACCEPT equ 43
SYS_WRITE equ 1
SYS_READ equ 0
SYS_CLOSE equ 3

section '.text' executable
_start:
    ; Создаём сокет
    mov     rax, SYS_SOCKET
    mov     rdi, AF_INET
    mov     rsi, SOCK_STREAM
    xor     rdx, rdx 
    syscall 
    mov     [server_sock], rax

    ; Связываем сокет с адресом сервера
    mov     rdi, rax 
    mov     rax, SYS_BIND
    mov     rsi, serv_addr
    mov     rdx, serv_addr_len
    syscall

    ; Установка очереди
    mov     rax, SYS_LISTEN
    mov     rdi, [server_sock]
    mov     rsi, Q_SIZE
    syscall

    ; Ждём подключения
    again:
    mov     rax, SYS_ACCEPT
    mov     rdi, [server_sock]
    xor     rsi, rsi 
    xor     rdx, rdx
    syscall
    mov     [client_sock], rax 

    ; Получаем сообщение от клиента
    mov     rdi, rax 
    xor     rax, rax 
    mov     rsi, buf 
    mov     rdx, BUF_SIZE
    syscall

    ; Отправляем сообщение обратно
    mov     rax, SYS_WRITE
    mov     rdi, [client_sock]
    syscall

    ; Закрываем соединение с клиентом
    mov     rax, SYS_CLOSE
    mov     rdi, [client_sock]
    
    ; Запускаем бесконечный цикл
    jmp     again 

section '.data' writable
    serv_addr   dw AF_INET         ; Семейство адресов
                db 0xEA,0x60      ; ПОРТ = 60000
                db 0,0,0,0          ; Адрес - любой
                db 0,0,0,0,0,0,0,0  ; Резервные нули
    serv_addr_len = $ - serv_addr   ; Размер структуры

    server_sock dq 0 ; Дескриптор серверного сокета
    client_sock dq 0 ; Дескриптор клиентского сокета

    buf rb BUF_SIZE
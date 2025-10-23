#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#define TIMEOUT 5

volatile sig_atomic_t got_signal = 0;  // cờ báo hiệu có tín hiệu

void sigint_handler(int sig){
    got_signal = 2;
    printf("SIGINT received\n");
}


int main(void){
    signal(SIGINT, sigint_handler);
    
    while(1){
        struct timeval tv;
        fd_set readfs;
        int ret = -1;

        // Khởi tạo tập hợp readfs và thêm mô tả file stdin vào readfs
        FD_ZERO(&readfs);
        
        FD_SET(STDIN_FILENO, &readfs);

        // Thiết lập timeout 
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        // Tập hợp mô tả file writefds và exceptfdc truyền vào NULL
        ret = select(STDIN_FILENO + 1, &readfs, NULL, NULL, &tv);

        if(ret == -1){
            perror("select()");
            exit(EXIT_FAILURE);
        } else if (ret){
            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin)){
                printf("Da nhap %s\n", buffer);
            }
            
        }
        else printf("Hay nhap mat khau cua ban: \n");
    }
    return 0;
}



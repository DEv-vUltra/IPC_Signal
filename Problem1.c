#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>



long unsigned int count = 0;

// Hàm xử lí tín hiệu số SIGINT (Ctrl+C)


void sigint_handler(int sig){
    count++;
    printf("\nCaught signal %d.", sig);
    if(count >= 3){
        printf("Receive SIGINT %ld times. Exit\n", count);
        exit(0);
    }
}



int main(){
    // Đăng kí một hàm xử lí cho tín hiệu SIGINT (Signal interrupt)
    // signal(SIGINT, sigint_handler);
    // Đăng kí một hàm xử lí cho tín hiệu SIGTSTP (Signal Terminal Stop)
    signal(SIGTSTP, sigint_handler);
    
    printf("Program is running. Press Ctrl+C to send SIGINT...\n");

    // lặp 3 lần trước khi dừng
    while(1){
        printf("Waiting for signal...\n");
        sleep(2); // delay 2s để quan sát
    }
    return 0;

}







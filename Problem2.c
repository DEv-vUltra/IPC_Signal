#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

long unsigned int count = 0; 
void sigalrm_handler(int sig){
    count++; 
    printf("Timer: %ld seconds\n", count);
    if (count >= 10){
        printf("Hoan thanh 10s. Ket thuc chuong trinh\n");
        exit(0);
    }

    // alarm(1);
    // Biến đếm tăng dần 1 đơn vị và nếu tăng đến 10 thì dừng
    // đặt alarm(1) để nó lặp lại từng giây
}


int main(){
    signal(SIGALRM, sigalrm_handler); //Đăng kí hàm xử lí cho tín hiệu SIGALRM

    alarm(10); // Đặt bộ đếm thời gian
    printf("Waiting for signal...\n");
    while(1){
        pause(); // Tạm dừng để chờ tín hiệu 
    }
    
    return 0;

}
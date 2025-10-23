#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>



void sigusr_handler(int sig){
    if(sig == SIGUSR1){
        printf("Nhan duoc tin hieu SIGUSR1\n");
    } else{
        printf("Khong nhan duoc tin hieu SIGUSR1\n");
    }
}


int main(){
    pid_t pid = fork();

    
    
    
    if(pid < 0){
        perror("fork failed");
        return 1;
    }else if (pid == 0){
        
        printf("This is child process.My PID = %d\n", getpid());
        signal(SIGUSR1, sigusr_handler);
        while(1){
            pause();
        }
        return 0;

    }
    else{
        printf("This is parent process. My PID = %d\n",getpid());
        for(int i = 0; i < 5; i++){
            sleep(2);
            if(kill(pid, SIGUSR1) == -1){
                perror("Kill failed");
                break;
            }
            else printf("Parent: send SIGUSR1 %d to child process\n",i+1);
        }
        sleep(2);
        kill(pid,SIGTERM);
        if(waitpid(pid, NULL, 0) == -1) perror("waitpid");
        else printf("Exit\n");
    }
    return 0;
}
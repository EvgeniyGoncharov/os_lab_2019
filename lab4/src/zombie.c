
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == 0){
        printf("Дочерний процесс завершился.\n");
        exit(0);
    }
    else if (child_pid > 0){
        printf("Родительский процесс создал дочерний процесс с PID %d.\n", child_pid);
        sleep(10);
    }
    else{
        perror("ошибка при fork");
        return 1;
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "a2_helper.h"
#include <semaphore.h>
#include <pthread.h>

#define NR_THREADS 4

pthread_t tids[NR_THREADS];
int ok=0;

void *threadFn(void *unused){
    int arg = *(int *) unused;
    if((3 == arg || 4 == arg) && ok == 0){
        info(BEGIN, 3, 4);
        info(BEGIN, 3, 3);
        info(END, 3, 3);
        info(END, 3, 4);
        ok=1;
    }
    else if(4 != arg){
        info(BEGIN, 3, arg);
        info(END, 3, arg);
    }
    return NULL;
}

int main()
{
    init();
    pid_t pid2;
    pid_t pid3;
    pid_t pid4;
    pid_t pid5;
    pid_t pid6;
    pid_t pid7;
    info(BEGIN, 1, 0);
    pid2 = fork();
    if (-1 == pid2)
    {
        perror("Nu s-a creat p2");
        return 1;
    }
    else if (0 == pid2)
    {
        info(BEGIN, 2, 0);
        info(END, 2, 0);
        return 0;
    }
    else{
        wait(NULL);
    }
    pid3 = fork();
    if(-1 == pid3){
        perror("Nu s-a creat p3");
        return 1;
    }
    else if(0 == pid3){
        info(BEGIN, 3, 0);
        for(int i=1;i<=4;i++){
            pthread_create(&tids[i], NULL, threadFn, &i);
            pthread_join(tids[i], NULL);
        }
        pid4 = fork();
        if(-1 == pid4){
            perror("Nu s-a creat p4");
            return 1;
        }
        else if(0 == pid4){
            info(BEGIN, 4, 0);
            pid7 = fork();
            if(-1 == pid7){
                perror("Nu s-a creat p7");
                return 1;
            }
            else if(0 == pid7){
                info(BEGIN, 7, 0);
                info(END, 7, 0);
                return 0;
            }
            else{
                wait(NULL);
            }
            info(END, 4, 0);
            return 0;
        }
        else{
            wait(NULL);
        }
        pid5 = fork();
        if(-1 == pid5){
            perror("Nu s-a creat p5");
            return 1;
        }
        else if(0 == pid5){
            info(BEGIN, 5, 0);
            pid6 = fork();
            if(-1 == pid6){
                perror("Nu s-a creat p6");
                return 1;
            }
            else if(0 == pid6){
                info(BEGIN, 6, 0);
                info(END, 6, 0);
                return 0;
            }
            else{
                wait(NULL);
            }
            info(END, 5, 0);
            return 0;
        }
        else{
            wait(NULL);
        }
        info(END, 3, 0);
        return 0;
    }
    else{
        wait(NULL);
    }
    info(END, 1, 0);
    return 0;
}
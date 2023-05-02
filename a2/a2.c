#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "a2_helper.h"

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
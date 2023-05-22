#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "RESP_PIPE_93348"
#define FIFO_NAME1 "REQ_PIPE_93348"

#define VARIANT_MESSAGE "VARIANT$"
#define VARIANT_MESSAGE_R "VARIANT$"
#define VARIANT_MESSAGE_R1 "VALUE$"

int main()
{
    unsigned int fd = -1;
    unsigned int fd1 = -1;

    if(mkfifo(FIFO_NAME, 0600) != 0){
        perror("ERROR\ncannot create the response pipe");
        return 1;
    }
    fd1 = open(FIFO_NAME1, O_RDONLY);
    if(fd1 == -1) {
        perror("ERROR\ncannot open the request pipe");
        return 1;
    }
    fd = open(FIFO_NAME, O_WRONLY);
    if(fd == -1) {
        perror("ERROR\ncannot open the response pipe");
        return 1;
    }
    const char* message = "START$";
    write(fd, message, strlen(message));
    printf("SUCCESS\n");

    int ok=0;
    char Message[50];
    int nr=93348;
    int i=0;
    read(fd1, &Message[i], 1);
    while(Message[i]!='$'){
        i++;
        read(fd1, &Message[i], 1);
    }
    while(ok==0){
        if(strcmp(Message, VARIANT_MESSAGE)==0){
            write(fd, VARIANT_MESSAGE_R, strlen(VARIANT_MESSAGE_R));
            write(fd, &nr, sizeof(nr));
            write(fd, VARIANT_MESSAGE_R1, strlen(VARIANT_MESSAGE_R1));
        }
        ok=1;
    }
    close(fd1);
    close(fd);
    unlink(FIFO_NAME);
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define FIFO_NAME "RESP_PIPE_93348"
#define FIFO_NAME1 "REQ_PIPE_93348"

#define VARIANT_MESSAGE "VARIANT$"
#define VARIANT_MESSAGE_R "VARIANT$"
#define VARIANT_MESSAGE_R1 "VALUE$"
#define CREATE_SHM_MESSAGE "CREATE_SHM$"
#define MAP_FILE_MESSAGE "MAP_FILE$"
#define EXIT_MESSAGE "EXIT$"

int main()
{
    unsigned int fd = -1;
    unsigned int fd1 = -1;

    if (mkfifo(FIFO_NAME, 0600) != 0)
    {
        perror("ERROR\ncannot create the response pipe");
        return 1;
    }
    fd1 = open(FIFO_NAME1, O_RDONLY);
    if (fd1 == -1)
    {
        perror("ERROR\ncannot open the request pipe");
        return 1;
    }
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("ERROR\ncannot open the response pipe");
        return 1;
    }
    const char *message = "START$";
    write(fd, message, strlen(message));
    printf("SUCCESS\n");

    int ok = 0;
    char Message[50];
    int nr = 93348;
    int i = 0;
    read(fd1, &Message[i], 1);
    while (Message[i] != '$')
    {
        i++;
        read(fd1, &Message[i], 1);
    }
    while (ok == 0)
    {
        if (strcmp(Message, VARIANT_MESSAGE) == 0)
        {
            write(fd, VARIANT_MESSAGE_R, strlen(VARIANT_MESSAGE_R));
            write(fd, &nr, sizeof(nr));
            write(fd, VARIANT_MESSAGE_R1, strlen(VARIANT_MESSAGE_R1));
        }
        else if (strcmp(Message, CREATE_SHM_MESSAGE) == 0)
        {
            int shmFd;
            int nr;
            read(fd1, &nr, sizeof(unsigned int));
            shmFd = shm_open("/7Crieg", O_CREAT | O_RDWR, 0664);
            ftruncate(shmFd, nr);
            if (shmFd < 0)
            {
                write(fd, CREATE_SHM_MESSAGE, strlen(CREATE_SHM_MESSAGE));
                write(fd, "ERROR$", strlen("ERROR$"));
            }
            else
            {
                write(fd, CREATE_SHM_MESSAGE, strlen(CREATE_SHM_MESSAGE));
                write(fd, "SUCCESS$", strlen("SUCCESS$"));
            }
        }
        else if (strcmp(Message, MAP_FILE_MESSAGE) == 0)
        {
            char text[50];
            read(fd1, &text[i], 1);
            while (text[i] != '$')
            {
                i++;
                read(fd1, &text[i], 1);
            }
            int fd2;
            off_t size;
            char *data = NULL;
            fd2 = open(text, O_RDONLY);
            if(fd2 == -1){
                write(fd, MAP_FILE_MESSAGE, strlen(MAP_FILE_MESSAGE));
                write(fd, "ERROR$", strlen("ERROR$"));
            }
            size = lseek(fd2, 0, SEEK_END);
            lseek(fd2, 0, SEEK_SET);
            data=(char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd2, 0);
            if(data == (void*)-1){
                write(fd, MAP_FILE_MESSAGE, strlen(MAP_FILE_MESSAGE));
                write(fd, "ERROR$", strlen("ERROR$"));
            }
            else{
                write(fd, MAP_FILE_MESSAGE, strlen(MAP_FILE_MESSAGE));
                write(fd, "SUCCESS$", strlen("SUCCESS$"));
            }
        }
        else if(strcmp(Message, EXIT_MESSAGE) == 0){
            close(fd1);
            close(fd);
            unlink(FIFO_NAME);
        }
        ok = 1;
    }
    return 0;
}
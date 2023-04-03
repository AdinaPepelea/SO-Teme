#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void continut(const char *path, char* name_ends_with){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    int ok=1, j=0, size=0, i=0, size1=0;

    dir = opendir(path);
    if(dir == NULL) {
        printf("Could not open directory\n");
        return;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0) {
                    if(S_ISDIR(statbuf.st_mode) || S_ISREG(statbuf.st_mode)) {
                        /*for(int i=strlen(entry->d_name)-strlen(name_ends_with);i<strlen(entry->d_name);i++){
                            if(entry->d_name[i]!=name_ends_with[j]){
                                ok=0;
                            }
                            j++;
                        }*/
                        while(entry->d_name[i]!='\0'){
                            size++;
                            i++;
                        }
                        while(name_ends_with[j]!='\0'){
                            size1++;
                            j++;
                        }
                        j=0;
                        for(int i=size-size1;i<size;i++){
                            if(entry->d_name[i]!=name_ends_with[j]){
                                ok=0;
                            }
                            j++;
                        }
                        if(ok==1){
                            printf("%s\n", fullPath);
                        }
                    }
                }
        }
    }   
    closedir(dir);
}

void continut_recursiv(const char *path, char* name_ends_with){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    int ok=1, j=0;

    dir = opendir(path);
    if(dir == NULL) {
        printf("Could not open directory");
        return;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0) {
                    if(S_ISDIR(statbuf.st_mode) || S_ISREG(statbuf.st_mode)) {
                        for(int i=strlen(entry->d_name)-strlen(name_ends_with);i<strlen(entry->d_name);i++){
                            if(entry->d_name[i]!=name_ends_with[j])
                                ok=0;
                            j++;
                        }
                        if(ok==1){
                            printf("%s\n", fullPath);
                        }
                        continut_recursiv(path, name_ends_with);
                    }
                }
        }
    }   
    closedir(dir);
}
/*
void parse(const char *path){
    char MAGIC;
    int VERSION;
    int NO_OF_SECTIONS;
    int SECT_TIPE;
    int fd=-1;
    fd=open(path, O_RDONLY);
    if(-1==fd){
        printf("Could not open input file\n");
    }
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    for(int i=0;i<size;i++){
        if(read(fd, &MAGIC, 1)!=1){
            printf("READING ERROR\n");
            break;
        }
        else if(MAGIC!='I') printf("Wrong magic\n");
    }
}
*/
int main(int argc, char **argv){
    const char *path=NULL;
    char* name_ends_with=NULL;
    if(strcmp(argv[1], "variant")==0)
    {
        printf("93348\n");
    }
    else if(strcmp(argv[1], "list")==0 && strcmp(argv[2], "path=")==0 && strcmp(argv[3], "name_ends_with=")==0){
        path=argv[2]+strlen("path=");
        name_ends_with=argv[3]+strlen("name_ends_with=");
        printf("SUCCESS\n");
        continut(path, name_ends_with);
    }
    else if(strcmp(argv[1], "list")==0 && strcmp(argv[2], "path=")==0 && strcmp(argv[3], "name_ends_with=")==0 && strcmp(argv[4], "recursive")==0){
        path=argv[2]+strlen("path=");
        name_ends_with=argv[3]+strlen("name_ends_with=");
        printf("SUCCESS\n");
        continut(path, name_ends_with);
    }
    return 0;
}
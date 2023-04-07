#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int permission=0;
int parse1=0;

void continut(const char *path, char* name_ends_with){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\ninvalid directory path");
        return;
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0) {
                    if(S_ISREG(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)){
                        if(permission==1){
                            if(S_IWUSR & statbuf.st_mode){
                                printf("%s\n", fullPath);
                            }
                        }
                        else if(strcmp(name_ends_with,"")!=0 && strstr(entry->d_name, name_ends_with)!=NULL && strcmp(strstr(entry->d_name, name_ends_with), name_ends_with)==0){
                            printf("%s\n", fullPath);
                        }else{
                            if(strcmp(name_ends_with,"")==0)
                            printf("%s\n", fullPath);
                        }
                    }
                }
        }
    }   
    closedir(dir);
}

void parse(const char *path){
    char MAGIC;
    unsigned short VERSION;
    unsigned char  NO_OF_SECTIONS;
    unsigned short SECT_TIPE;
    unsigned short HEADER_SIZE;
    char name[16];
    int size;
    int offset;
    int fd=-1;
    fd=open(path, O_RDONLY);
    if(-1==fd){
        printf("Could not open input file\n");
    }
    lseek(fd, -1, SEEK_END);
    read(fd, &MAGIC, 1);
    lseek(fd, -3, SEEK_END);
    read(fd, &HEADER_SIZE, 2);
    lseek(fd, -HEADER_SIZE, SEEK_END);
    read(fd, &VERSION, 2);
    read(fd, &NO_OF_SECTIONS, 1);
    if(MAGIC!='I'){
        printf("ERROR\n wrong magic");
        exit(-1);
    }
    if(VERSION<67 || VERSION>177){
        printf("ERROR\n wrong version");
        exit(-1);
    }
    if(NO_OF_SECTIONS<2 || NO_OF_SECTIONS>16){
        printf("ERROR\n wrong sect_nr");
        exit(-1);
    }
    lseek(fd, 15, SEEK_CUR);
    for(int i=0;i<NO_OF_SECTIONS;i++){
        read(fd, &SECT_TIPE, 2);
        lseek(fd, 23, SEEK_CUR);
        if(SECT_TIPE!=46 && SECT_TIPE!=37 && SECT_TIPE!=17 && SECT_TIPE!=85){
            printf("ERROR\n wrong sect_types");
            exit(-1);
        }
}
    printf("SUCCESS\n");
    printf("version=%d\n", VERSION);
    printf("nr_sections=%d\n", NO_OF_SECTIONS);
    lseek(fd, -HEADER_SIZE+3, SEEK_END);
    for(int i=0;i<NO_OF_SECTIONS;i++){
        read(fd, &name, 15);
        name[15]='\0';
        read(fd, &SECT_TIPE, 2);
        read(fd, &offset, 4);
        read(fd, &size, 4);
        printf("section%d: %s %d %d\n", i+1, name, SECT_TIPE, size);
    }
    close(fd);
}

void findall(const char *path){
    char MAGIC;
    unsigned short VERSION;
    unsigned char  NO_OF_SECTIONS;
    unsigned short SECT_TIPE;
    unsigned short HEADER_SIZE;
    char name[16];
    int size;
    int offset;
    int fd=-1;
    fd=open(path, O_RDONLY);
    if(-1==fd){
        printf("Could not open input file\n");
    }
    lseek(fd, -1, SEEK_END);
    read(fd, &MAGIC, 1);
    lseek(fd, -3, SEEK_END);
    read(fd, &HEADER_SIZE, 2);
    lseek(fd, -HEADER_SIZE, SEEK_END);
    read(fd, &VERSION, 2);
    read(fd, &NO_OF_SECTIONS, 1);
    if(MAGIC!='I'){
        return;
    }
    if(VERSION<67 || VERSION>177){
        return;
    }
    if(NO_OF_SECTIONS<2 || NO_OF_SECTIONS>16){
        return;
    }
    lseek(fd, 15, SEEK_CUR);
    for(int i=0;i<NO_OF_SECTIONS;i++){
        read(fd, &SECT_TIPE, 2);
        lseek(fd, 23, SEEK_CUR);
        if(SECT_TIPE!=46 && SECT_TIPE!=37 && SECT_TIPE!=17 && SECT_TIPE!=85){
            return;
        }
    }
    lseek(fd, -HEADER_SIZE+3, SEEK_END);
    for(int i=0;i<NO_OF_SECTIONS;i++){
        read(fd, &name, 15);
        read(fd, &SECT_TIPE, 2);
        read(fd, &offset, 4);
        read(fd, &size, 4);
        if(size>953){
            return;
        }
    }
    printf("%s\n",path);
    close(fd);
}

void continut_recursive(const char *path, char* name_ends_with){
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\ninvalid directory path");
        return;
    }
    
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0) {
                        if(permission==1){
                            if(S_IWUSR & statbuf.st_mode){
                                printf("%s\n", fullPath);
                            }
                        }
                        if(parse1==0){
                            if(strcmp(name_ends_with,"")!=0 && strstr(entry->d_name, name_ends_with)!=NULL && strcmp(strstr(entry->d_name, name_ends_with), name_ends_with)==0){
                                printf("%s\n", fullPath);
                            }else{
                            if(strcmp(name_ends_with,"")==0)
                                printf("%s\n", fullPath);
                            }
                        }
                        else{
                            if(S_ISREG(statbuf.st_mode)){
                                    findall(fullPath);
                    }
                        }
                    if(S_ISDIR(statbuf.st_mode))
                    {
                        continut_recursive(fullPath, name_ends_with);
                    }
                }
        }
    }   
    closedir(dir);
}

int main(int argc, char **argv){
    char path[512]="";
    char name_ends_with[512]="";
    int recursive=0;

    if(strcmp(argv[1], "variant")==0)
    {
        printf("93348\n");
    }
    else if(strstr(argv[1], "list")!=NULL){
        for(int i=2; i<argc;i++){
            if(strstr(argv[i], "recursive")){
                recursive=1;
            }
            if(strstr(argv[i],"path=")!=NULL){
                strcpy(path,argv[i]+5);
            }
            if(strstr(argv[i], "name_ends_with=")!=NULL){
                strcpy(name_ends_with, argv[i]+15);
            }
            if(strstr(argv[i], "has_perm_write")!=NULL){
                permission=1;
            }
        }
        if(recursive==0){
            continut(path, name_ends_with);
        }
        else {
            printf("SUCCESS\n");
            continut_recursive(path, name_ends_with);
        }
    }
   else if(strstr(argv[1], "parse")!=NULL)
   {
    for(int i=2;i<argc;i++){
        if(strstr(argv[i],"path=")!=NULL){
                strcpy(path,argv[i]+5);
        }
    }
    parse(path);
   }
   else if(strstr(argv[1], "findall")!=NULL){
    parse1=1;
    for(int i=2;i<argc;i++){
        if(strstr(argv[i],"path=")!=NULL){
                strcpy(path,argv[i]+5);
        }
   }
   printf("SUCCESS\n");
   continut_recursive(path, name_ends_with);
   }
    return 0;
}
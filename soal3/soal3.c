#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
#include<ctype.h>

pthread_mutex_t signal;

char *checkName(char *dir){
    char *name = strrchr(dir, '/');
    if(name == dir)
        return "";
    return name + 1;
}

char *lowercase(char *str){
    unsigned char *temp = (unsigned char *)str;

    while(*temp){
        *temp = tolower(*temp);
        temp++;
    }
    return str;
}

char *checkExt(char *dir){
    char *unk = {"Unknown"};
    char *ext = strrchr(dir, '.');
    if(ext != NULL)
        return lowercase(ext + 1);
    return unk;
}

void* categorize(void *arg){
    char *src = (char *)arg;                        // source path, can change due to modifications in arg
    char srcP[150];                                 // source path, doesn't change if arg is modified
    memcpy(srcP, (char*)arg, 150);
    char *srcExt = checkExt(src);                   // extension of file yg akan dibuat folder diseragamkan lowercase
    
    DIR *dir = opendir(srcExt);
    if(dir == NULL){
        mkdir(srcExt, S_IRWXU);
    }

    char *srcName = checkName(srcP);                // source name + extension
    char *curr = getenv("PWD");
    
    char *destP = malloc(strlen(srcExt) + 1 + strlen(srcName) + 1 + strlen(curr) + 1);
    strcpy(destP, curr);
    strcat(destP, "/");
    strcat(destP, srcExt);
    strcat(destP, "/");
    strcat(destP, srcName);
    
    pthread_mutex_lock(&signal);
    if(rename(srcP, destP) != 0){
        fprintf(stderr,"error: %s\n\n",strerror(errno));
    }
    pthread_mutex_unlock(&signal);
}

void categorizeFolder(char *folderPath, int threadSize){
    DIR *fd = opendir(folderPath);
    struct dirent *dp;
    pthread_t tid[threadSize];
    int count = 0;
    
    while(((dp = readdir(fd)) != NULL)){
        if(dp->d_type == DT_REG){
            char *fileName = malloc(strlen(folderPath) + 1 + strlen((char*)(dp->d_name)) + 1);
            sprintf(fileName, "%s/%s", folderPath, dp->d_name);
            pthread_create(&tid[count], NULL, categorize, (void *)fileName);
            count++;
        }
    }

    for(count=0; count<threadSize; count++){
        pthread_join(tid[count], NULL);
    }

    closedir(fd);
}


int main(int argc, char *argv[]){
    if(argc == 0){
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1], "-f") == 0){

        pthread_t tid[argc-2];
        int count = 0;

        for(int i=2; i<argc; i++){
            pthread_create(&tid[count], NULL, categorize, (void *)argv[i]);
            count++;
        }

        count = 0;
        
        for(count=0; count<argc-2; count++){
            pthread_join(tid[count], NULL);
        }

        return 0;

    }else if(strcmp(argv[1], "-d") == 0){

        DIR *fd = opendir(argv[2]);
        struct dirent *dp;
        int threadSize = 0;

        while(((dp = readdir(fd)) != NULL)){
            if(dp->d_type == DT_REG){
                threadSize++;
            }
        }

        categorizeFolder(argv[2], threadSize);

    }else{

        char *curr = getenv("PWD");
        struct dirent *dp;
        DIR *dir = opendir(curr);
        int threadSize = 0;

        while((dp = readdir(dir)) != NULL){
            if(dp->d_type == DT_REG){
                threadSize++;
            }
        }

        categorizeFolder(curr, threadSize);
        
    }

}


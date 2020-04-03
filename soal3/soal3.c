#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>

char *checkExt(char *dir){
    char *ext = strrchr(dir, '.');
    if(ext == dir)
        return "";
    return ext + 1;
}

char *checkName(char *dir, char *ext){
    char *name = strrchr(dir, '/');
    if(name == dir)
        return "";
    name = name + 1;
}

void* categorize(void *arg){
    char *srcP = (char *)arg;
    char *srcExt = checkExt(srcP);
    char *srcName = checkName(srcP, srcExt);
    
    DIR *dir = opendir(srcExt);
    char *curr = getenv("PWD");

    if(dir == NULL){
        mkdir(srcExt, S_IRWXU);
    }

    char *destP = malloc(strlen(srcExt) + 1 + strlen(srcName) + 1 + strlen(curr) + 1);
    strcpy(destP, curr);
    strcat(destP, "/");
    strcat(destP, srcExt);
    strcat(destP, "/");
    strcat(destP, srcName);

    if(rename(srcP, destP) != 0){
        fprintf(stderr,"error: %s\n",strerror(errno));
    }
}

void categorizeFiles(char **arg, int total){
    int i, count = 0;
    pthread_t tid[total];

    for(i=2; i<total; i++){
        pthread_create(&tid[count], NULL, categorize, (void *)arg[i]);
        count++;
    }

    for(count=0; count<total; count++){
        pthread_join(tid[count], NULL);
    }
}

void categorizeFolder(char *folderName){
    DIR *dir = opendir(folderName);
    struct dirent *dp;
    int count = 0, size = 0;

    while((dp = readdir(dir)) != NULL)
        size++;
    
    pthread_t tid[size];
    count = 0;

    while((dp = readdir(dir)) != NULL){
        char *fileName = malloc(strlen(folderName) + 1 + strlen(dp->d_name) + 1);
        sprintf(fileName, "%s/%s", folderName, dp->d_name);
        pthread_create(&tid[count], NULL, categorize, (void *)fileName);
        count++;
    }

    for(count=0; count<size; count++){
        pthread_join(tid[count], NULL);
    }
}

void categorizeAll(){
    char *curr = getenv("PWD");
    struct dirent *dp;
    DIR *dir = opendir(curr);
    int sumFiles = 0;

    while((dp = readdir(dir)) != NULL){
        if(dp->d_type == DT_REG){
            sumFiles++;
        }else if(dp->d_type == DT_DIR){
            char *name = malloc(strlen(curr) + 1 + strlen(dp->d_name) + 1);
            sprintf(name, "%s/%s", curr, dp->d_name);
            categorizeFolder(name);
        }
    }

    pthread_t tid[sumFiles];
    int count = 0;

    while((dp = readdir(dir)) != NULL){
        if(dp->d_type == DT_REG){
            char *name = malloc(strlen(curr) + 1 + strlen(dp->d_name) + 1);
            sprintf(name, "%s/%s", curr, dp->d_name);
            pthread_create(&tid[count], NULL, categorize, (void *)name);
            count ++;
        }
    }

    count = 0;

    for(count=0; count<sumFiles; count++){
        pthread_join(tid[count], NULL);
    }
}

int main(int argc, char *argv[]){
    if(argc == 0){
        exit(EXIT_FAILURE);
    }

    if(strcmp(argv[1], "-f") == 0){
        categorizeFiles(argv, argc-2);
    }else if(strcmp(argv[1], "-d") == 0){
        categorizeFolder(argv[2]);
    }else if(strcmp(argv[1], "*") == 0){
        categorizeAll();
    }

}


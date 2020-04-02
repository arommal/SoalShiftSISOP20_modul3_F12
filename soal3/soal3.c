#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>

char* checkExt(char *fileName){
    char *ext = strrchr(fileName, '.');
    if(ext == '.') return "";
    return ext + 1;
}

void* categorizeFiles(void *arg){
    char *fileName = (char *)arg;
    char *fileExt = checkExt(fileName);
    mkdir(fileExt, S_IRWXU);
    chdir()
    rename(fileName, fileExt);
}

void* categorizeFolder(void *arg){
    int p[2];
    char *argv_ls[] = {"ls", (char *)arg, NULL};
    char *argv_wc[] = {"wc", "-l", NULL};

    char **cmd[] = { argv_ls, argv_wc, NULL };
    
    pipeline(cmd);
}

void* categorizeAll(void *arg){

}

int main(int argc, char *argv[]){
    if(argc == 0){
        exit(EXIT_FAILURE);
    }

    int i = 2, count = 0;
    pthread_t tid[argc - 1];

    if(strcmp(argv[1], "-f") == 0){
        while(i < argc){
            pthread_create(tid[count++], NULL, categorizeFiles, (void *)argv[i]);
            i++;
        }

        count = 0;

        for(count=1; count<argc-1; count++){
            pthread_join(tid[count], NULL);
        }
    }else if(strcmp(argv[1], "-d") == 0){
        pthread_create(tid, NULL, categorizeFolder, (void *)argv[2]);
    }else if(strcmp(argv[1], "\*") == 0){
        pthread_create(tid, NULL, categorizeAll, (void *)argv[i]);
    }
}


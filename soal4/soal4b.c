#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

int it = 0;
pthread_mutex_t signal;

void* operate(void *arg){
    pthread_mutex_lock(&signal);
    int val = *((int*)arg), i;
    long long res = 0;
    
    for(i=1; i<=val; i++){
        res += i;
    }

    if((it+1) % 5 == 0){
        printf("%lld\n", res);
    }else{
        printf("%lld ", res);
    }
    it++;
    pthread_mutex_unlock(&signal);
    return NULL;
}

int main(){
    int i;
    long long fact;
    pthread_t tid[20];
    
    key_t key = 1234;
    int shmid = shmget(key, 20*sizeof(int), IPC_EXCL);
    int *res = (int*)shmat(shmid, 0, 0);
    
    for(i=0; i<20; i++){
        printf("%d ", res[i]);
        if((i+1) % 5 == 0)
            printf("\n");
    }

    printf("\n");

    int count = 0;

    for(i=0; i<20; i++){
        int *a = malloc(sizeof(*a));
        *a = res[i];
        pthread_create(&tid[i], NULL, operate, a);
        count++;
    }

    for(i=0; i<20; i++){
        pthread_join(tid[i], NULL);
    }

    shmdt((void*)res);
    shmctl(shmid, IPC_RMID, NULL);
}
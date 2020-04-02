#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

void* factorial(void *arg){
    int val = *((int*)arg), i;
    long long res = 1;

    if(val==0 || val==1)
        res = 1;
    else{
        for(i=0; i<val; i++){
            res *= (val - i);
        }
    }
    printf("%lld ", res);

    pthread_exit(0);
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

    int count = 0;

    for(i=0; i<20; i++){
        int *a = malloc(sizeof(*a));
        *a = res[i];
        pthread_create(&tid[i], NULL, factorial, a);
        count++;
    }

    for(i=0; i<20; i++){
        pthread_join(tid[i], NULL);
    }

    shmdt((void*)res);
    shmctl(shmid, IPC_RMID, NULL);
}
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

void* multiply(void *arg){
    int *val = (int *)arg;
    int i, k = 0;

    int x = val[0];

    for(i=1; i<=x; i++)
        k += val[i]*val[i+x];
    
    int *p = (int*)malloc(sizeof(int));
    
    *p = k;

    pthread_exit(p);
}

int main(){
    int m = 4, n = 2, o = 2, p = 5, i, j, k;
    int matA[4][2] = {
                        {2, 2},
                        {3, 2},
                        {3, 4},
                        {1, 3}
                    };

    int matB[2][5] = {
                        {3, 1, 4, 2, 1},
                        {2, 2, 1, 3, 2}
                    };

    int max = m*p;

    pthread_t *thread = (pthread_t*)malloc(max*sizeof(pthread_t));

    int count = 0;
    int* data = NULL;

    for(i=0; i<m; i++){
        for(j=0; j<p; j++){
            data = (int*)malloc((20)*sizeof(int));
            data[0] = o;

            for(k=0; k<n; k++)
                data[k+1] = matA[i][k];

            for(k=0; k<o; k++)
                data[k+n+1] = matB[k][j];
            
            pthread_create(&thread[count++], NULL, multiply, (void*)data);
        }   
    }

    int *res;
    key_t key = 1234;
    int shmid = shmget(key, 20*sizeof(int), IPC_CREAT | 0666);
    res = (int*)shmat(shmid, 0, 0);
    
    for(i=0; i<max; i++){
        void *k;
        pthread_join(thread[i], &k);
        int *q = (int*)k;
        printf("%d\t", *q);
        if((i+1) % p == 0)
            printf("\n");
        
        res[i] = *q;
    }

    shmdt((void*)res);
}
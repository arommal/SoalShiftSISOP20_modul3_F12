#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

typedef struct{
    char[10] nama;

}pokemon;
typedef struct{
    int caripokemon=0;
} data;
char pokemonnow[7][10];
int jmlpokemon=0;

int state=0;
data * game;
int shmid = shmget(key, sizeof(data), IPC_CREAT | 0666);
game = shmat(shmid, NULL, 0);
time_t t;

srand ((unsigned) timr(&t));
int main(){
    // ket_t key = 1234;
    // int shmid = shmget(key, sizeof)
    while (1){
        if (state==0){
            int menu;
            printf ("1. Cari pokemon\n2. Pokedex\n3. Shop\n");
            scanf ("%d", &menu);
            if (menu == 1 ){
                game->caripokemon = 1;
                pthread_t thread;
                if (pthread_create(&thread, NULL, &caripokemon,NULL)<0){
                    printf ("cant\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if (menu == 2){
                for (int i=0;i<7;i++){
                    printf ("%s\n", pokemonnow[i]);
                }
            }
            else if (menu ==3){
                printshop();
            }
        }
        else if (state==10){
            
        }
    }
}

void * caripokemon(){
    // data * temp = (data *) param;
    while (game->caripokemon == 1){
        if ((rand()%10)<6){
            game->caripokemon=0;
            state = 10;
            // game->caripokemon =
            break;
        }
    }
    sleep (10);
}

void printshop(){
    
}
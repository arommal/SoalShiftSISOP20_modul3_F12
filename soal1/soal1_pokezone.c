#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct WildPokemon_s{
	int isLocked;
	char name[20];
	int type;
	int price;
	int capRate;
	int escapeRate;
}WildPokemon_t;

typedef struct Shop_s{
	int isLocked;
	int lullabyPowder;
	int pokeball;
	int berry;
}Shop_t;

typedef struct GameData_s{
	int isRunning;
	WildPokemon_t * sharedPokemon;
	Shop_t * sharedShop;

}GameData_t;

char arr[20];
void * randomizePokemonThread(void * param);
void * shopThread(void * param);


time_t t;
srand ((unsigned) timr(&t));

int main(){	
	//shared memory
	key_t key = 1111;
	WildPokemon_t * sharedPokemon;
	int shmid =  shmget(key, sizeof(WildPokemon_t), IPC_CREAT | 0666);
	sharedPokemon = shmat(shmid, NULL, 0);
	memset(sharedPokemon, 0, sizeof(sharedPokemon));

	key_t key2 = 2222;
	Shop_t * sharedShop;
	int shmid2 =  shmget(key2, sizeof(Shop_t), IPC_CREAT | 0666);
	sharedShop = shmat(shmid2, NULL, 0);
	memset(sharedShop, 0, sizeof(sharedShop));

	GameData_t * gameData = malloc(sizeof(GameData_t));
	memset(gameData, 0, sizeof(GameData_t));
    gameData->isRunning = 1;
	gameData->sharedShop = sharedShop;
    gameData->sharedPokemon = sharedPokemon;

    //thread
    pthread_t thread1, thread2;
 	if(pthread_create(&thread1, NULL, &randomizePokemonThread, (void*)gameData ) < 0){
		printf("can't create thread\n");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&thread2, NULL, &shopThread, (void*)gameData ) < 0){
		printf("can't create thread\n");
		exit(EXIT_FAILURE);
	}  
    
    printf("Pokezone\n1. Shutdown game\nChoice : ");
    scanf("%s", arr);
    if (strcmp(arr, "shutdown")==0){
        pid_t id = fork();
		if(!id){
			FILE * pidProcess = popen("pidof soal1_traizone", "r");
			int pida = 0;
			while(fscanf(pidProcess, "%d", &pida) != EOF){
				id = fork();
				if(!id){
					char pid[10];
					snprintf(pid, sizeof(pid), "%d", pida);
					char * argv[] = {"kill", "-9", pid, NULL};
					execv("/bin/kill", argv);
				}
			}
			char pid[10];
			snprintf(pid, sizeof(pid), "%d", ppp);
			char * argv[] = {"kill", "-9", pid, NULL};
			execv("/bin/kill", argv);
		}
    }

	pthread_join(pokemonThread, NULL);
	pthread_join(shop, NULL);

	shmdt(sharedPokemon);
    shmctl(shmid, IPC_RMID, NULL);

	shmdt(sharedShop);
    shmctl(shmid2, IPC_RMID, NULL);
	return 0;
}

void * randomizePokemonThread(void * param){
	GameData_t * gameData = (GameData_t *)param;
	WildPokemon_t * sharedPokemon = gameData->sharedPokemon;

    while(gameData->isRunning){
		if (sharedPokemon->isLocked){
			continue;
		}
        int x= rand()%100;
        if (x<=79){
            sharedPokemon->type = 1;
            sharedPokemon->escapeRate = 5;
            sharedPokemon->capRate = 70;
            sharedPokemon->price = 80;
            int y = rand()%5;
            memset (sharedPokemon->name,0,sizeof(sharedPokemon->name));
            if (y==0) strcpy (sharedPokemon->name, "Bulbasaur");
            else if (y==1) strcpy (sharedPokemon->name, "Charmender");
            else if (y==2) strcpy (sharedPokemon->name, "Squirtle");
            else if (y==3) strcpy (sharedPokemon->name, "Rattata");
            else if (y==4) strcpy (sharedPokemon->name, "Caterpie");
        }
        else if (x<=94){
            sharedPokemon->type = 2;
            sharedPokemon->escapeRate = 10;
            sharedPokemon->capRate = 70;
            sharedPokemon->price = 100;
            int y = rand()%5;
            memset (sharedPokemon->name,0,sizeof(sharedPokemon->name));
            if (y==0) strcpy (sharedPokemon->name, "Pikachu");
            else if (y==1) strcpy (sharedPokemon->name, "Eevee");
            else if (y==2) strcpy (sharedPokemon->name, "Jigglypuff");
            else if (y==3) strcpy (sharedPokemon->name, "Snorlax");
            else if (y==4) strcpy (sharedPokemon->name, "Dragonite");
        }
        else if (x<=99){
            sharedPokemon->type = 3;
            sharedPokemon->escapeRate = 20;
            sharedPokemon->capRate = 50;
            sharedPokemon->price = 200;
            int y = rand()%5;
            memset (sharedPokemon->name,0,sizeof(sharedPokemon->name));
            if (y==0) strcpy (sharedPokemon->name, "Mew");
            else if (y==1) strcpy (sharedPokemon->name, "Mewtwo");
            else if (y==2) strcpy (sharedPokemon->name, "Moltres");
            else if (y==3) strcpy (sharedPokemon->name, "Zapdos");
            else if (y==4) strcpy (sharedPokemon->name, "Articuno");
        }
        int z=rand()%8000;
        if (z==0){
            sharedPokemon->escapeRate += 5;
            sharedPokemon->capRate -= 20;
            sharedPokemon->price += 5000;
        }
    }

    // int isLocked;
	// char name[20];
	// int type;
	// int price;
	// int capRate;
	// int escapeRate;
}

void * shopThread(void * param){
	GameData_t * gameData = (GameData_t *)param;
	gameData->sharedShop->lullabyPowder = 100;
	gameData->sharedShop->berry = 100;
	gameData->sharedShop->pokeball = 100;
	// fflush(stdout);
	while(gameData->isRunning){
		sleep(10);
		gameData->sharedShop->berry += 10;
		if(gameData->sharedShop->berry > 200){
			gameData->sharedShop->berry = 200;
		}
		gameData->sharedShop->pokeball += 10;
		if(gameData->sharedShop->pokeball > 200){
			gameData->sharedShop->pokeball = 200;
		}
		gameData->sharedShop->lullabyPowder += 10;
		if(gameData->sharedShop->lullabyPowder > 200){
			gameData->sharedShop->lullabyPowder = 200;
		}
		// fflush(stdout);
	}
}
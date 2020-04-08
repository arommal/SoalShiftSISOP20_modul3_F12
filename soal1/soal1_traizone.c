#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

typedef struct Pokemon_s{
	int released;
	char name[20];
	int type;
	int ap;
	int price;
}Pokemon_t;

typedef struct GameData_s{
	int isRunning;
	int jumlahLullabyActive;
	WildPokemon_t * sharedPokemon;
	Shop_t * sharedShop;
}GameData_t;

Pokemon_t pokemon[7];
int pokecount = 0;

int pokeBallc=10;
int lullabyPowderc=0;
int berryc=0;
int pokedollar=0;

int screen;
int cariPokemon=0;

void * cariPokemonThread(void * param);
void * wildpokemonThread(void * param);
void * lullabyThread(void * param);
void * pokemonThread(void * param);

char arr[20];
time_t t;
srand ((unsigned) timr(&t));
int *isrunning;
int state=0;
int paramisrunning;
int main(){
    key_t key = 1111;
	WildPokemon_t * sharedPokemon;
	int shmid =  shmget(key, sizeof(WildPokemon_t), IPC_CREAT | 0666);
	sharedPokemon = shmat(shmid, NULL, 0);

	key_t key2 = 2222;
	Shop_t * sharedShop;
	int shmid2 =  shmget(key2, sizeof(Shop_t), IPC_CREAT | 0666);
	sharedShop = shmat(shmid2, NULL, 0);

    GameData_t * gameData = malloc(sizeof(GameData_t));
	memset(gameData, 0, sizeof(GameData_t));
	gameData->sharedShop = sharedShop;
    gameData->sharedPokemon = sharedPokemon;

    *isRunning = &gameData->isRunning;
	*isRunning = 1;
    while (*isRunning){
        if (state==0){
            printf ("Normal Mode\n");
            if (caripokemon) printf("1. Berhenti cari\n2. Pokedex\n3. Shop\nChoice : ");
            else printf("1. Cari pokemon\n2. Pokedex\n3. Shop\nChoice : ");
            memset(arr,0,sizeof(arr));
            scanf("%s",arr);
            if (strcmp(arr,"cari")==0){
                if (caripokemon) cariPokemon=0;
                else{
                    caripokemon = 1;
                    pthread_t thread;
                    if(pthread_create(&thread, NULL, &cariPokemonThread, (void *)gameData) < 0){
                        printf("can't create thread\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else if (strcmp(arr,"pokedex")==0) state=1;
            else if (strcmp(arr,"shop")==0) state=2;
        }
        else if (state==1){
            int i;
            printf ("Pokedex\n");
            if (pokecount==0) printf("Anda tidak memiliki pokemon\n");
            else {
                for (i=0;i<pokecount;i++) printf ("Name = %s - AP = %d\n",pokemon[i].name, pokemon[i].ap);
            }

            printf ("\nAnda memiliki %d Berry\n1. Beri berry\n2/ Lepas pokemon\n3. Kembali ke normal mode\nChoice : ");
            memset(arr,0,sizeof(arr));
            scanf("%s",arr);
            if (strcmp(arr,"berry")==0){
                if (pokecount==0) printf("Anda tidak memiliki pokemon\n");
                else {
                    if (berryc>=1){
                        berryc--;
                        for(i=0;i<pokecount;i++) pokemon[i].ap+=10;
                        printf ("Berry telah diberikan\n");
                    }
                    else printf("Anda tidak memiliki berry\n");
                }
            }
            else if (strcmp(arr,"lepas")==0){
                if (pokecount==0) printf("Anda tidak memiliki pokemon\n");
                else{
                    int x;
                    printf ("Masukan index pokemon yang anda ingin lepas : ");
                    scanf("%d",&x);
                    pokecount--;
                    pokemon[x].released=1;
                }
            }
            else if (strcmp(arr,"kembali")==0) state=0;
        }
        else if (state == 2){
            printf ("Shop\n");
            Shop_t * sharedShop = gameData->sharedShop;
            printf("List item :\n1. Lullaby Powder - Stok : %d - Harga : 60\n",sharedShop->lullabyPowder);
            printf("2. Pokeball - Stok : %d - Harga : 5\n",sharedShop->pokeball);
            printf("3. Berry - Stok : %d - Harga : 15\n",sharedShop->berry);
            printf ("Anda memiliki :\n");
            printf ("%d Pokedollar - %d Lullaby Powder - %d Pokeball - %d Berry\n", pokedollar, lullabyPowderc, pokeBallc, berryc);
            printf ("\n1. Belli Lullaby Powder\n2. Beli Pokeball\n3. Beli Berry\n4. Kembali ke normal mode\nChoice : ");
            int z;
            scanf ("%d",&z);
            if (z==1){
                if (pokedollar < 60) printf ("Uang tidak cukup");
                else {
                    if (sharedShop->lullabyPowder > 0){
                        printf ("Lullaby powder telah dibeli\n");
                        lullabyPowderc++;
                        pokedollar-=60;
                        sharedShop->lullabyPowder--;
                    }
                    else printf("Stok habis\n");
                }
            }
            else if (z==2){
                if (pokedollar < 5) printf ("Uang tidak cukup");
                else {
                    if (sharedShop->pokeball > 0){
                        printf ("pokeball telah dibeli\n");
                        pokeballc++;
                        pokedollar-=5;
                        sharedShop->pokeball--;
                    }
                    else printf("Stok habis\n");
                }
            }
            else if (z==3){
                if (pokedollar < 15) printf ("Uang tidak cukup");
                else {
                    if (sharedShop->berry > 0){
                        printf ("pokeball telah dibeli\n");
                        berryc++;
                        pokedollar-=15;
                        sharedShop->berry--;
                    }
                    else printf("Stok habis\n");
                }
            }
            else if (z==4) state = 0;
        }
        else if (state == 3){
            WildPokemon_t * wpokemon = gameData->sharedPokemon;
	        wpokemon->isLocked = 1;
	        pthread_t wild;
            paramisrunning=1;
            if(pthread_create(&wild, NULL, &wildpokemonThread, (void *)gameData) < 0){
                printf("can't create thread\n");
                exit(EXIT_FAILURE);
            }
            while (wpokemon->isLocked){
                printf ("Capture Mode\n")
                printf ("Pokemon : %s - Rarity : %d\n", wpokemon->name, wpokemon->type);
                printf ("\n1. Tangkap\n2. Gunakan lullaby\n3. Keluar\nChoice : ");
                int z;
                scanf ("%d",&z);
                if (pokemon->isLocked==0){
                    printf("Pokemon kabur\n");
                    break;
                }
                if (z==1){
                    if (pokeballc > 0){
                        pokeballc--;
                        int random = rand()%100;
                        if (random< wpokemon->capRate + gameData->jumlahLullabyActive * 2){
                            if (pokecount ==6) printf ("Slot pokemon full\n");
                            else{
                                pokecount++;
                                pokemon[pokecount]->released = 0;
                                strcpy(pokemon[pokecount]->name,wpokemon->name);
                                pokemon[pokecount]->type = wpokemon->type;
                                pokemon[pokecount]->price = wpokemon->price;
                                pthread_t t;
                                if(pthread_create(&t, NULL, &pokemonThread, (void *) pokecount) < 0){
                                    printf("can't create thread\n");
                                    exit(EXIT_FAILURE);
                                }
                                printf ("%s tertangkap!\n",wpokemon->name);
                                wpokemon->isLocked =0;
                                state = 0;
                            }
                        }
                        else printf ("tidak tertangkap\n");
                    }
                    else printf ("Anda tidak memiliki pokeball\n")
                }
                else if(z==2){
                    if (lullabyPowderc > 0){
                        lullabyPowderc--;
                        pthread_t thread;
                        if(pthread_create(&lullaby, NULL, &lullabyThread,  (void *)gameData) < 0){
                            printf("can't create thread\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else printf ("Anda tidak memiliki lullaby powder!\n");
                }
                else if (z==3){
                    wpokemon->isLocked = 0;
                    state = 0;
                }
            }
            wpokemon->isLocked = 0;
        }
    }
}

void * cariPokemonThread(void * param){
	GameData_t * gameData = (GameData_t *) param;
	while(gameData->cariPokemon){
		if(rand()%10 <= 5){
			cariPokemon = 0;
			state = 3;
			break;
		}
		sleep(10);
	}
}

void * wildpokemonThread(void * param){
	GameData_t * gameData = (GameData_t *) param;
	while (gameData->isRunning && paramisrunning){
		sleep(20);
		if(paramisrunnin || !gameData->isRunning){
			break;
		}
		if(gameData->jumlahLullabyActive) continue;
		int random = rand()%100;
		if(random < gameData->sharedPokemon->escapeRate){
			gameData->sharedPokemon->isLocked = 0;
			break;
		}
	}
	
}

void * lullabyThread(void * param){
	GameData_t * gameData = (GameData_t *)param;
	gameData->jumlahLullabyActive++;
	sleep(10);
	gameData->jumlahLullabyActive--;
}

void * pokemonThread(void * param){
    int * x = (int *)param;
	while (*isrunning && !pokemon[x]->released){
		sleep(10);
		if(state ==3)continue;
		pokemon[x]->ap -= 10;
		if(pokemon[x]->ap == 0){
			int random = rand()%10;
			if(random == 0){
				pokemon[x]->ap = 50;
			}else{
				pokemon[x]->released = 1;
                pokecount--;
			}
		}
	}
	
}
# SoalShiftSISOP20_modul3_F12
1. Di normal mode kita bisa mencari pokemon, melihat pokedex, dan melihat shop
untuk dapat mencari pokemon kita membuat sebuah thread caripokemon
```
pthread_t thread;
if(pthread_create(&thread, NULL, &cariPokemonThread, (void *)gameData) < 0){
  printf("can't create thread\n");
  exit(EXIT_FAILURE);
}
```
dimana caripokemon thread berisi
```
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
```
disini kita melakukan random, jika ternyata <=5 (60% chance) maka kita akan masuk ke state 3, dimana state 3 adalah capture mode
di dalam capture mode kita meminta data wild pokemon dari pokezone, dimana di dalam sana ada thread randomize pokemon yang kita jalankan
```
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
}
```
Thread ini adalah untuk merandom mulai dari rarity, nama pokemon, dan kemungkinan shiny pokemon. Perlu diingat bahwa ini kita simpan dalam shared memory. Ketika kita bertemu pokemon, kita bisa memilih untuk menangkap pokemon atau menggunakan lullaby untuk menambah chance penangkapan pokemon yang kami implementasikan seperti berikut
```
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
                    else printf ("Anda tidak memiliki pokeball\n");
```
Ketika pokemon berhasil ditangkap, maka kita akkan menyimpan pokemon tersebut dalam daftar pokemon yang telah kita punya, dan kemudiang menjalankan sebuah thread lagi yang mengurangi ap pokemon setiap 10 detik dengan implementasi
```
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
```
kemudian untuk penggunaan lullaby powder implementasinya adalah
```
                    if (lullabyPowderc > 0){
                        lullabyPowderc--;
                        pthread_t thread;
                        if(pthread_create(&lullaby, NULL, &lullabyThread,  (void *)gameData) < 0){
                            printf("can't create thread\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else printf ("Anda tidak memiliki lullaby powder!\n");
```
Disini, ketika kita mennggunakan lullaby powder, kita akan menjalankan sebuah thread lagi yang menambahkan jumlah lulaby yang aktif saat ini dengan implementasi
```
void * lullabyThread(void * param){
	GameData_t * gameData = (GameData_t *)param;
	gameData->jumlahLullabyActive++;
	sleep(10);
	gameData->jumlahLullabyActive--;
}
```
dan itu tadi menutup semua fitur yang ada di dalam menangkap pokemon, kemudian kita ke pilihan yang kedua yaitu pokedex, disini kita diminta untuk mencetak semua pokemon yang kita punya dan kita bisa memberi berry pada pokemon dan melepas pokemon. Untuk mencetak pokemon implementasinya cukup sederhhana yaitu
```
            if (pokecount==0) printf("Anda tidak memiliki pokemon\n");
            else {
                for (i=0;i<pokecount;i++) printf ("Name = %s - AP = %d\n",pokemon[i].name, pokemon[i].ap);
            }
```
Untuk penggunaan berry dan pelepasan pokemon implementasinya adalah sebagai berikut
```
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
```
Kemudian di fitur yang ketiga adalah shop, disini saya juga mennggunakan shared memory dari pokezone, dimana di dalam pokezone kita memiliki thread yang menambahkan stok barang dengan 10 untuk setiap 10 detik dengan implementasi
```
void * shopThread(void * param){
	GameData_t * gameData = (GameData_t *)param;
	gameData->sharedShop->lullabyPowder = 100;
	gameData->sharedShop->berry = 100;
	gameData->sharedShop->pokeball = 100;
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
	}
}
```
Di shop sendiri kita bisa membeli lullaby powder, pokeball dan berry dengan implementasi
```
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
```
Kemudian di pokezone kita diminta untuk dapat melakukan shutdown dengan kill semua proses yang berhubungan dnegan program ini, dengan implementasi
```
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
```


2. 

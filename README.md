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


2. Untuk nomor 2, kita menggunakan client dan socket dan juga threading, sehingga di server kita dapat menerima client dengan
```
    while (1){
        for (v=1;v<=2;v++){
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            int temp = new_socket;

            if (v==1) socket1= new_socket;
            else socket2=new_socket;
            pthread_t thread;
            iret = pthread_create(&thread, NULL, &mulai, (void *)&temp);
            if (iret){
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
                exit(EXIT_FAILURE);
            }
        }
    }
 ```
Dimana untuk setiap client kita akan menjalanka thread mulai yang berisi program utama yang akan dijalankan setiap client, untuk client sendiri kita akan dimulai untuk memilih login atau register, untuk register 
```
if (strcmp("register", arr) == 0){
                printf ("Username : ");
                // scanf("%s",key);
                getchar();
                scanf ("%[^\n]s", key);
                // gets (key);
                send (sock, key, strlen(key),0);
                memset (buffer, 0 , 1024);
                valread = read(sock, buffer,1024);

                printf ("Password : ");
                // scanf("%s",key);
                getchar();
                scanf ("%[^\n]s", key);
                // gets (key);
                send (sock, key, strlen(key),0);
                memset (buffer, 0 , 1024);
                valread = read(sock, buffer,1024);

                    printf ("register success\n");
            }
```
Disini terlihat bahwa kita menggunakan `` scanf ("%[^\n]s", key);``, dimana program ini memungkinkan untuk menerima spasi sebagai username dan password, seperti `sisop 2020` atau sebagainya. Kemudian ktia mengirimkan uname dan pass tersebut ke server dan server akan melakukan penyimpanan ke `akun.txt` dengan implementasi
```
                memset (buffer, 0 , 1024);
                valread = read( socketbaru , buffer, 1024);
                file = fopen("akun.txt", "a");
                if (file == NULL){
                    file = fopen("./akun.txt", "w");
                }
                fputs (buffer, file);
                fputs ("\n",file);
                fclose (file);
                send (socketbaru, hello, strlen(hello),0);

                memset (buffer, 0 , 1024);
                valread = read( socketbaru , buffer, 1024);

                file = fopen("akun.txt", "a");
                fputs (buffer, file);
                fputs ("\n",file);
                fclose (file);
```
disini juga ketika register berhasil dilakukan maka kita melakukan pencetakkan username dan passowrd untuk semua akun yang telah registrasi
```
                file = fopen("akun.txt", "r");
                char a[50], b[50];
                memset (uname,0, sizeof (uname));
                memset (pass,0, sizeof (pass));
                while (fgets (a,50,file) != NULL){
                    if (fgets (b,50,file) != NULL){
                        printf ("%s%s",a,b);
                    }
                }	
                fclose (file);
```
Untuk login, untuk di sisi client adalah kurang lebih sama yaitu mengirimkan username dan passowrd ke server, namun bedanya di akhir, server akan mengirimkan jika username dan password yang diinputkan adalah benar, sedhingga di server kita melakukan pengecekkan sebagai berikut
```
                while (fgets (a,50,file) != NULL){
                    if (fgets (b,50,file) != NULL){
                        
                        strcpy (uname[x], a);
                        strcpy (pass[x], b);
                        // printf ("%s%s%s%s",a,b,uname[x],pass[x]);
                        x++;
                    }
                }	
                fclose (file);
```
disini saya menyimpan uname dan pass ke array dan melakukan pengecekan
```
                for (i=1;i<x;i++){
                    if (strcmp(buffer,uname[i])==0){
                        // printf ("masuk\n");
                        jml++;
                        y[jml] = i;
                    }
                }
		
		 for (i=1;i<=jml;i++){
                    if (strcmp(buffer,pass[y[i]])==0){
                        flag=1;
                    }
                }
```
sehingga ketika flag = 1 maka kita mengirim ke client bahwa login berhasil
```
                if (flag == 1){
                    printf ("Auth success\n");
                    send (socketbaru, acc, strlen(acc),0);
                }
                else {
                    printf ("Auth failed\n");
                    send (socketbaru, hello, strlen(hello),0); 
                }
```
Setelah login berhasi, kita bisa melkukan find match dimana ketika client melakukan findmatch maka ia akan mengirimkan sebuah pesan ke server, dan ketika server menerima pesan tersebut, jumlah pemain akan ditambahkann. Ketika jumlah player adalah sudah kelipatan 2, maka kita mulai pertandignan, jika belum, maka kita kirim pesan wait ke client. Ketika mulai bertanding, kita buat 2 thread di client, 1 untuk menembak dan 1 untuk ditembak, dengan implementasi
```
void * kena(void * param){
    
    while (hp>0){
        memset (buffer, 0 , 1024);
        valread = read(sock, buffer,1024);
        if (strcmp(buffer, "end")==0){
            hp=0;
            break;
        }
        else if (strcmp (buffer, "kena")==0){
            hp-=10;
            printf ("hp = %d\n",hp);
        }
        send(sock, lala, strlen(lala),0);
    }
}
int count=0;
void * tembak(void * param){
    // memset (buffer, 0 , 1024);
    // valread = read(sock, buffer,1024);
    char c;
    while (c=getch()){
        if (fight==0) break;
        if (strcmp(buffer,"end")==0 || hp<=0) break;
        // char c = getch();
        if (c==' '){
            count++;
            printf ("hit !!\n");
            send (sock, hit, strlen(hit),0);
            // memset (buffer, 0 , 1024);
            // valread = read(sock, buffer,1024);
            if (count==10){
                printf ("Game berakhir kamu menang\n");
                fight=0;
                break;
            }
            if (strcmp(buffer,"end")==0 || hp<=0) break;
        }
       
        //  else send (sock, nothit, strlen(nothit),0);
    }
}
```
Disini kita menggunakan ``getch()`` untuk menerima input spasi, dan ketika itu terjadi kita mengirimkan pesan ke server bahwa ia menembak dan di server akan mengurangi darah dari hp lawan
```
                                if (strcmp(buffer, "hit")==0){
                                    if (socketbaru == socket1) {
                                        hp2-=10;
                                        if (hp2==0){
                                            send(socket2, end, strlen(end),0);
                                            send(socket1, end, strlen(end),0);
                                            break;
                                        }
                                        else {
                                            send (socket2, kena, strlen(kena),0);
                                        }
                                        // memset (buffer, 0 , 1024);
                                        // valread = read( socketbaru , buffer, 1024);
                                        // send (socket1, hello, strlen(hello),0);
                                    }
                                    else{
                                        hp1-=10;
                                        if (hp1==0){
                                            send(socket1, end, strlen(end),0);
                                            send(socket2, end, strlen(end),0);
                                            break;
                                        }
                                        else {
                                            send (socket1, kena, strlen(kena),0);
                                            
                                        }
```
dan di thread ditembak , kita hanya mencatat hp kita sekarang, dan melakukan pencetakkan hp untuk setiap kali ditembak, dan ketika hp suda 0, kita mengirimkan pesan ke server bahwa kita telah kehabisan hp, dan server akan mengirimkan pesan ke dua client yang mana yang emnang dan yang mana yang kalah
```
                            if (hp1<=0) {
                                send (socket1, lose, strlen(lose),0);
                                send (socket2, win, strlen(win),0);
                            }
                            else {
                                send (socket1, win, strlen(win),0);
                                send (socket2, lose, strlen(lose),0);
                            }
```
Di client akan menerima dan mencetak sesuai dengan pesan yang diterima dari server
```
                memset (buffer, 0 , 1024);
                valread = read(sock, buffer,1024);
                if (strcmp(buffer, "lose")==0) printf ("Game berakhir, kamu kalah\n");
                else if (strcmp(buffer, "win")==0) printf ("Game berakhir, kamu menang\n");
```

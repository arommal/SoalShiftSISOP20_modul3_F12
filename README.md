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

3. Pada soal 3, kita diminta untuk mengkategorikan file berdasarkan format. Terdapat 3 kemungkinan penggunaan program ini berdasarkan argumen yang digunakan.

- Argumen **-f**, mengkategorikan file-file yang dinyatakan pathnya setelah -f
Cara melakukannya adalah sebagai berikut:
a. Membuat thread sebanyak jumlah file yang dinyatakan pathnya
b. Thread melakukan fungsi `categorize` di bawah ini untuk tiap path file
```C
void* categorize(void *arg){
    char *src = (char *)arg;                        // source path, can change due to modifications in arg
    char srcP[150];                                 // source path, doesn't change if arg is modified
    memcpy(srcP, (char*)arg, 150);
    char *srcExt = checkExt(src);                   // extension of file yg akan dibuat folder diseragamkan lowercase
    
    DIR *dir = opendir(srcExt);
    if(dir == NULL){
        mkdir(srcExt, S_IRWXU);
    }

    char *srcName = checkName(srcP);                // source name + extension
    char *curr = getenv("PWD");
    
    char *destP = malloc(strlen(srcExt) + 1 + strlen(srcName) + 1 + strlen(curr) + 1);
    strcpy(destP, curr);
    strcat(destP, "/");
    strcat(destP, srcExt);
    strcat(destP, "/");
    strcat(destP, srcName);
    
    pthread_mutex_lock(&signal);
    if(rename(srcP, destP) != 0){
        fprintf(stderr,"error: %s\n\n",strerror(errno));
    }
    pthread_mutex_unlock(&signal);
}
```
c. Menjoinkan thread

- Argumen **-d**, mengkategorikan file-file di sebuah folder yang pathnya ditulis setelah -d
Cara melakukannya adalah sebagai berikut:
a. Membuka direktori yang dimaksud dengan memanggil `opendir()`, menginisiasi struktur pointer dirent untuk mengiterasi direktori tersebut, dan menginisiasi variabel counter untuk menghitung jumlah file yang terdapat di direktori sebagai `threadSize`
```C
DIR *fd = opendir(argv[2]);
struct dirent *dp;
int threadSize = 0;
```
b. Mengiterasi isi direktori dan menentukan tipenya, jika merupakan file, maka `threadSize` ditambah 1.
```C
while(((dp = readdir(fd)) != NULL)){
    if(dp->d_type == DT_REG){
        threadSize++;
    }
}
```
c. Memanggil fungsi `categorizeFolder()` yang berisi:
```C
void categorizeFolder(char *folderPath, int threadSize){
    DIR *fd = opendir(folderPath);
    struct dirent *dp;
    pthread_t tid[threadSize];
    int count = 0;
    
    while(((dp = readdir(fd)) != NULL)){			// mengiterasi direktori yang dituju sekali lagi dan
        if(dp->d_type == DT_REG){				// membuat thread categorize untuk masing2 isi bertipe file
            char *fileName = malloc(strlen(folderPath) + 1 + strlen((char*)(dp->d_name)) + 1);
            sprintf(fileName, "%s/%s", folderPath, dp->d_name);
            pthread_create(&tid[count], NULL, categorize, (void *)fileName);
            count++;
        }
    }

    for(count=0; count<threadSize; count++){
        pthread_join(tid[count], NULL);
    }

    closedir(fd);
}
```

- Argumen **\***, mengkategorikan file-file yang terdapat di current directory
Cara melakukannya adalah sebagai berikut:
a. Menentukan current directory
```C
char *curr = getenv("PWD");
```
b. Membuka current directory dengan memanggil `opendir()`, menginisiasi struktur pointer dirent untuk mengiterasi current directory, dan menginisiasi variabel counter untuk menghitung jumlah file yang terdapat di current directory sebagai `threadSize`.
```C
struct dirent *dp;
DIR *dir = opendir(curr);
int threadSize = 0;
```
c. Mengiterasi isi direktori dan menentukan tipenya, jika merupakan file, maka `threadSize` ditambah 1.
```C
while((dp = readdir(dir)) != NULL){
    if(dp->d_type == DT_REG){
          threadSize++;
    }
}
```
d. Memanggil fungsi `categorizeFolder()`

4. Pada soal 4, terdapat 3 permasalahan, 2 di antaranya membutuhkan penggunaan thread.
***SOAL PERTAMA***
a. Kita diminta untuk mendefinisikan 2 matriks masing-masing berukuran 4 x 2 dan 2 x 5 dan melakukan operasi perkalian matriks terhadap keduanya. Sebagai satu contoh kita mendefinisikan matriks-matriks tersebut sebagai berikut:
```C
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
```
b. Diketahui hasil dari perkalian A(4,2) dan B(2,5) adalah M(4,5), yang berarti terdapat 4 baris, 5 kolom, dan 4 x 5 = 20 elemen.
c. Kita membuat array data berukuran 20 yang akan menyimpan sementara nilai masing-masing elemen dari matriks hasil.
d. Array data tersebut diisi dengan elemen matriks A dan elemen matriks B yang akan dikali.
```C
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
```
e. Proses pengaliannya dilakukan dengan thread `multiply()` yang dipanggil dengan mem-passing isi array data.
```C
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
```
f. Karena kita butuh berbagi data matriks hasil perkalian dengan program lain, dibutuhkan pembuatan shared memory untuk matriks hasil tersebut
```C
    key_t key = 1234;
    int shmid = shmget(key, 20*sizeof(int), IPC_CREAT | 0666);
    res = (int*)shmat(shmid, 0, 0);
```
g. Pencetakan matriks hasil dilakukan dengan memanggil argumen data yang telah dipassing di thread `multiply()` sebelumnya
```C
    for(i=0; i<max; i++){
        void *k;
        pthread_join(thread[i], &k);
        int *q = (int*)k;
        printf("%d\t", *q);
        if((i+1) % p == 0)
            printf("\n");
        
        res[i] = *q;
    }
```

***SOAL KEDUA***
a. Kita diminta untuk mengambil matriks hasil dengan shared memory dari program 4a dan mencetaknya
```C
key_t key = 1234;
int shmid = shmget(key, 20*sizeof(int), IPC_EXCL);
int *res = (int*)shmat(shmid, 0, 0);
    
for(i=0; i<20; i++){
    printf("%d ", res[i]);
    if((i+1) % 5 == 0)
        printf("\n");
}
```
b. Setelah itu, kita diminta untuk mengoperasikan masing-masing elemen matriks hasil dengan menambahkan elemen tersebut dengan nilai yang lebih rendah hingga mencapai nilai = 1. Kita melakukan ini menggunakan thread yang memanggil fungsi `operate()`
```C
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
```
Penggunaan mutex diperlukan agar data yang dicetak bisa beraturan sesuai dengan urutan elemen asli.
c. Membuat dan menjoinkan thread
```C
    for(i=0; i<20; i++){
        int *a = malloc(sizeof(*a));
        *a = res[i];
        pthread_create(&tid[i], NULL, operate, a);
        count++;
    }

    for(i=0; i<20; i++){
        pthread_join(tid[i], NULL);
    }
```
d. Mendetach shared memory yang berbagi data matriks hasil karena telah selesai digunakan di program ini.

***SOAL KETIGA***
a. Program ini harus dapat mengembalikan jumlah file yang terdapat di current directory menggunakan pipeline.
b. Program ini ekivalen dengan pengeksekusian command `ls | wc -l` pada terminal.
c. Pertama, menyimpan masing-masing command sebelum dan sesudah pipeline sebagai data bertipe char**, lalu menggabungkannya.
```C
int p[2];
char *argv_ls[] = {"ls", NULL};
char *argv_wc[] = {"wc", "-l", NULL};

char **cmd[] = { argv_ls, argv_wc, NULL };
```
d. Memanggil fungsi `pipeline()` yang membutuhkan data `char ***cmd`.
```C
void pipeline(char ***cmd){
	int fd[2];					// inisiasi file descriptor read dan write 
	pid_t pid;
	int fdd = 0;
	int files = 0;

	while (*cmd != NULL){
		pipe(fd);
        	pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(1);
		}else if (pid == 0) {			// child process menjalankan argumen pertama
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}else {					// parent process 
			wait(NULL);
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}
```

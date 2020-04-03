#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// int encrate = 0;
int escrate = 0;
int caprate = 0;
int pokedol = 0;
int rarity = 0;
char nama[10];
void randompokemon(){
    int x =rand()%100;
    if (x<=79){
        rarity = 1;
        escrate = 5;
        caprate = 70;
        pokedol = 80;
        int y = rand()%5;
        memset (nama,0,sizeof(nama));
        if (y==0) strcpy (nama, "Bulbasaur");
        else if (y==1) strcpy (nama, "Charmender");
        else if (y==2) strcpy (nama, "Squirtle");
        else if (y==3) strcpy (nama, "Rattata");
        else if (y==4) strcpy (nama, "Caterpie");
    }
    else if (x<=94){
        rarity = 2;
        escrate = 10;
        caprate = 50;
        pokedol = 100;
        int y = rand()%5;
        memset (nama,0,sizeof(nama));
        if (y==0) strcpy (nama, "Pikachu");
        else if (y==1) strcpy (nama, "Eevee");
        else if (y==2) strcpy (nama, "Jigglypuff");
        else if (y==3) strcpy (nama, "Snorlax");
        else if (y==4) strcpy (nama, "Dragonite");
    }
    else if (x<=99){
        rarity = 3;
        escrate = 20;
        caprate = 50;
        pokedol = 200;
        int y = rand()%5;
        memset (nama,0,sizeof(nama));
        if (y==0) strcpy (nama, "Mew");
        else if (y==1) strcpy (nama, "Mewtwo");
        else if (y==2) strcpy (nama, "Moltres");
        else if (y==3) strcpy (nama, "Zapdos");
        else if (y==4) strcpy (nama, "Articuno");
    }
}
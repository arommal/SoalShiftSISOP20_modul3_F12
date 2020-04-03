#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <termios.h>
#define PORT 8092
int fight;
char buffer[1024] = {0};
static struct termios old, new;
 int hp=100;
void initTermios(int echo)  {
  tcgetattr(0, &old);
  new = old;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new);
}
 
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

char getch(){
    char ch;
      initTermios(0);
    ch = getchar();
    resetTermios();
    return ch;
}
int sock = 0, valread,x=0;
void * kena(void * param);
void * tembak(void * param);
char * hit = "hit";
char * nothit = "lol";
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    // int sock = 0, valread,x=0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    int state = 1;
    char arr[10];
    char key[50];
    while (1){
        if (state == 1){
            printf ("1. Login\n2. Register\nChoices : ");
            scanf ("%s", arr);
            send (sock, arr, strlen(arr),0);
            memset (buffer, 0 , 1024);
            valread = read(sock, buffer,1024);
            if (strcmp("login", arr) == 0){
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

                if (strcmp("ya", buffer)==0){
                    printf ("login success\n");
                    state = 2;
                }
                else printf ("login failed\n");
            }
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

                // if (strcmp("ya", buffer)==0){
                    printf ("register success\n");
                    // state = 1;
                // }
                // else printf ("login success\n");
            }
        }
        else if (state == 2){
            hp=100;
            
            printf ("1. Find Match\n2. Logout\nChoices : ");
            scanf ("%s", arr);
            if (strcmp(arr, "logout") == 0){
                state = 1;
            }
            else if (strcmp (arr, "find") == 0){
                send (sock, arr, strlen(arr),0);
                memset (buffer, 0 , 1024);
                valread = read(sock, buffer,1024);
                while (strcmp(buffer,"wait")==0){
                    printf ("Waiting for player..\n");
                    memset (buffer, 0 , 1024);
                    valread = read(sock, buffer,1024);      
                    sleep(1);
                }
                // int hp=100;
                fight =1;
                while (fight==1){
                        printf ("game dimulai silahkan tap tap secepat mungkin\n");
                int temp = sock;
                pthread_t threadkena,threadnembak;
                int iret1,iret2;
                iret1 = pthread_create(&threadkena, NULL, &kena, (void *)&temp);
                if (iret1){
                    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
                    exit(EXIT_FAILURE);
                }

                iret2 = pthread_create(&threadnembak, NULL, &tembak, (void *)&temp);
                if (iret2){
                    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
                    exit(EXIT_FAILURE);
                }
                while (hp>0 && fight == 1){
                    if (strcmp(buffer,"end")==0) fight =0;
                }
                memset (buffer, 0 , 1024);
                valread = read(sock, buffer,1024);
                if (strcmp(buffer, "lose")==0) printf ("Game berakhir, kamu kalah\n");
                else if (strcmp(buffer, "win")==0) printf ("Game berakhir, kamu menang\n");
                fight =0;
                }
            }
        }
    }
    return 0;
}

char * lala = "lala";
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
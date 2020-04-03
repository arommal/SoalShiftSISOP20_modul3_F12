#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#define PORT 8092


    char uname[50][50];
    char pass[50][50];
    int x;
    int i;
    char temp1[50], temp2[50];
    char *acc = "ya";
    FILE *file;
    int socket1, socket2;
    int v;

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char arr[100];
    int iret;
    int jumlah=0;
    char *tunggu = "wait";
    int maen =0;
    static struct termios old, new;
 
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
void *mulai(void * param);

int main(int argc, char const *argv[]) {

      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

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
    return 0;
}

   void *mulai (void * param){
        int socketbaru = *(int *) param;
        // int a = socketbaru;
        while (1){
            memset (buffer, 0 , 1024);
            valread = read( socketbaru , buffer, 1024);
            send (socketbaru, hello, strlen(hello),0);
            if (strcmp (buffer, "register") == 0){

                memset (buffer, 0 , 1024);
                valread = read( socketbaru , buffer, 1024);
                // strcpy (uname[x], buffer);
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
                // strcpy (pass[x], buffer);
                // x++;
                // FILE *x;
                file = fopen("akun.txt", "a");
                fputs (buffer, file);
                fputs ("\n",file);
                fclose (file);

                // for (i=1;i<x;i++) printf ("%s\n%s\n", uname[i], pass[i]);
                // FILE *x;
                file = fopen("akun.txt", "r");
                char a[50], b[50];
                memset (uname,0, sizeof (uname));
                memset (pass,0, sizeof (pass));
                while (fgets (a,50,file) != NULL){
                    if (fgets (b,50,file) != NULL){
                        printf ("%s%s",a,b);
                        // strcpy (uname[x], a);
                        // strcpy (pass[x], b);
                        // x++;
                    }
                }	
                fclose (file);
                // int j;
                // for (i=1;i<x;i++) printf ("uname = %s\n pass = %s\n",uname[i],pass[i]);
                send (socketbaru, hello, strlen(hello),0);   
            }

            else if (strcmp (buffer, "login") == 0){
                x=1;
                file = fopen("akun.txt", "r");
                char a[50], b[50];
                memset (uname,0, sizeof (uname));
                memset (pass,0, sizeof (pass));
                while (fgets (a,50,file) != NULL){
                    if (fgets (b,50,file) != NULL){
                        
                        strcpy (uname[x], a);
                        strcpy (pass[x], b);
                        // printf ("%s%s%s%s",a,b,uname[x],pass[x]);
                        x++;
                    }
                }	
                fclose (file);
                int jml=0,y[50], flag=0;
                memset (y,0,50);
                memset (buffer, 0 , 1024);
                valread = read( socketbaru , buffer, 1024);
                strcat (buffer, "\n");
                // printf ("%s", buffer);
                for (i=1;i<x;i++){
                    if (strcmp(buffer,uname[i])==0){
                        // printf ("masuk\n");
                        jml++;
                        y[jml] = i;
                    }
                }
                send (socketbaru, hello, strlen(hello),0);

                memset (buffer, 0 , 1024);
                valread = read( socketbaru , buffer, 1024);
                strcat (buffer, "\n");
                for (i=1;i<=jml;i++){
                    if (strcmp(buffer,pass[y[i]])==0){
                        flag=1;
                    }
                }
                if (flag == 1){
                    printf ("Auth success\n");
                    send (socketbaru, acc, strlen(acc),0);
                }
                else {
                    printf ("Auth failed\n");
                    send (socketbaru, hello, strlen(hello),0); 
                }

                if (flag == 1){
                    memset (buffer, 0 , 1024);

                    valread = read( socketbaru , buffer, 1024);
                    if (strcmp(buffer, "find")==0){
                        jumlah++;
                        while (jumlah%2!=0){
                            // printf ("masuk\n");
                            send (socketbaru, tunggu, strlen(tunggu),0); 
                            sleep(1);
                        }
                        char * kena = "kena";
                        char * end = "end";
                        char * win = "win";
                        char * lose = "lose";

                        if (jumlah %2==0){
                            send (socketbaru, hello, strlen(hello),0); 
                            int hp1 =100, hp2=100;
                            while (hp1>0 && hp2>0){
                                memset (buffer, 0 , 1024);
                                valread = read( socketbaru , buffer, 1024);
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
                                        // memset (buffer, 0 , 1024);
                                        // valread = read( socketbaru , buffer, 1024);
                                        // send (socket2, hello, strlen(hello),0);
                                        // hp1-=10;
                                    }
                                    
                                }
                            }
                            printf ("selesai\n");
                            // send(socketbaru, end, strlen(end),0);
                            // memset (buffer, 0 , 1024);
                            // valread = read( socketbaru , buffer, 1024);
                            // send(socketbaru, end, strlen(end),0);
                            if (hp1<=0) {
                                send (socket1, lose, strlen(lose),0);
                                send (socket2, win, strlen(win),0);
                            }
                            else {
                                send (socket1, win, strlen(win),0);
                                send (socket2, lose, strlen(lose),0);
                            }
                        }
                    }
                }
            }
        }
    }
    
    void * kena (void * param){

    }

    void * tembak (void * param){

    }
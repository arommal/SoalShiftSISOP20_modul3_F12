#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<wait.h>

void pipeline(char ***cmd){
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int files = 0;

	while (*cmd != NULL){
		pipe(fd);
        pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(1);
		}else if (pid == 0) {
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}else {
			wait(NULL);
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}

int main(){
    int p[2];
    char *argv_ls[] = {"ls", NULL};
    char *argv_wc[] = {"wc", "-l", NULL};

    char **cmd[] = { argv_ls, argv_wc, NULL };
    
    pipeline(cmd);
}
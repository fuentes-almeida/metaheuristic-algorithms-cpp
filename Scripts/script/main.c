#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
	pid_t childPIDorZero=fork();
	if (childPIDorZero<0){
		perror("fork() error");
		exit(-1);
	}
	if (childPIDorZero !=0){
		printf("I'm the parent %d, my child is %d\n",getpid(),childPIDorZero);
		//printf("%s",command1);
		system("ssh c-0-11 './G_Fuentes/EvaluadorFSFAP2 G_Fuentes/GSM2-184.ctr 184 39'");
		wait(NULL);//wait for child process to joint with its parent
	}
	else{
		printf("I'm the child %d, my parent is %d\n",getpid(),getppid());
		system("ssh c-0-12 './G_Fuentes/EvaluadorFSFAP2 G_Fuentes/GSM2-184.ctr 184 39'");
		//execl("/home/juan-j2c/Dropbox/Metaheuristicas/script/EvaluadorFSFAP","EvaluadorFSFAP","GSM2-184.ctr","184","39",NULL);
	}
return 0;
}

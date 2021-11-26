#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	pid_t childPIDorZero=fork();
	if (childPIDorZero<0){
		perror("fork() error");
		exit(-1);
	}
	
	if (childPIDorZero !=0){
		printf("I'm the parent %d, my child is %d\n",getpid(),childPIDorZero);
		wait(NULL);//wait for child process to joint with its parent
	}
	else{
		printf("I'm the child %d, my parent is %d",getpid(),getppid());
		system("ssh c-0-10 '/home/user_demo/G_Fuentes/EvaluadorFSFAP GSM2-184.ctr 184 39'");
		//execl("/home/user_demo/G_Fuentes/EvaluadorFSFAP","EvaluadorFSFAP","GSM2-184.ctr","184","39",NULL);
	}
return 0; 
}

// Manuel Morales Amat 48789509T

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void callPSA(){
	printf("Soy el proceso A con pid %d, he recibido la señal.\n",getpid());
	if(fork()==0){
		if(execlp("ps", "ps", NULL)!=0){
			perror("No se ha podido ejecutar la orden");
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}
void callPSB(){
	printf("Soy el proceso B con pid %d, he recibido la señal.\n",getpid());
	if(fork()==0){
		if(execlp("ps", "ps", NULL)!=0){
			perror("No se ha podido ejecutar la orden");
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}
void callLSX(){
	printf("Soy el proceso X con pid %d, he recibido la señal.\n",getpid());
	if(fork()==0){
		if(execlp("ls", "ls", NULL)!=0){
			perror("No se ha podido ejecutar la orden");
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}
void callLSY(){
	printf("Soy el proceso Y con pid %d, he recibido la señal.\n",getpid());
	if(fork()==0){
		if(execlp("ls", "ls", NULL)!=0){
			perror("No se ha podido ejecutar la orden");
		}
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}
void alarma(){}
void Z(char diana, int time, int dpid,int Rpid, int Apid, int Bpid, int Xpid, int Ypid){
	int estado;
	printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(),Bpid,Apid,Rpid);
	signal(SIGALRM,alarma);
	alarm(time);
	pause();
	kill(dpid,SIGUSR1);
	printf("Soy Z(%d) y muero\n", getpid());
	kill(Ypid,SIGUSR2);
	exit(EXIT_SUCCESS);
}
void Y(int Rpid, int Apid, int Bpid, int Xpid){
	printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(),Bpid,Apid,Rpid);
	signal(SIGUSR1,callLSY);
	signal(SIGUSR2,alarma);
	pause();
	printf("Soy Y(%d) y muero\n", getpid());
	kill(Xpid,SIGUSR2);
	exit(EXIT_SUCCESS);
}
void X(int Rpid, int Apid, int Bpid){
	printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n",getpid(),Bpid,Apid,Rpid);
	signal(SIGUSR1,callLSX);
	signal(SIGUSR2,alarma);
	pause();
	printf("Soy X(%d) y muero\n", getpid());
	exit(EXIT_SUCCESS);
}
void B(char diana, int time, int dpid, int Rpid, int Apid){
	int i = 0;
	int Xpid, Ypid;
	int Bpid = getpid();
	printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n",getpid(),Apid,Rpid);
	signal(SIGUSR1,callPSB);
	for(i;i<3;i++){
		switch (i)
		{
		case 0:
			if(!(Xpid = fork())) X(Rpid,Apid,Bpid);
			break;
		case 1:
			if(!(Ypid = fork())) Y(Rpid,Apid,Bpid,Xpid);
			break;
		case 2:
			if(!fork()){ 
				switch (diana)
				{
				case 'A':
					dpid = Apid;
					break;
				case 'B':
					dpid = Bpid;
					break;
				case 'X':
					dpid = Xpid;
					break;
				case 'Y':
					dpid = Ypid;
					break;
				}
				Z(diana,time,dpid,Rpid,Apid,Bpid,Xpid,Ypid);
			}
			break;
		default:
			break;
		}
	}
	wait(NULL);
	printf("Soy B(%d) y muero\n", getpid());
	exit(EXIT_SUCCESS);
}
void A(char diana, int time){
	int rootpid, Apid, Bpid;
	rootpid = getppid();
	Apid = getpid();
	printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n",getpid(),getppid());
	signal(SIGUSR1,callPSA);
	if(!fork()){
		if(diana == 'A') 
			B(diana,time,Apid,rootpid,Apid);
		B(diana,time,0,rootpid,Apid);
	}
	wait(NULL);
	printf("Soy A(%d) y muero\n", getpid());
	exit(EXIT_SUCCESS);
}
void root(char diana, int time){
	printf("\nSoy el proceso ejec: mi pid es %d\n",getpid());
	if(!fork()){
		A(diana,time);
	}
	wait(NULL);
	printf("Soy ejec(%d) y muero\n", getpid());
}

int main(int argc, char ** argv){
	if(--argc == 2){
		root(argv[1][0],atoi(argv[2]));
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}

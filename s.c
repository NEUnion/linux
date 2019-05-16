#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/shm.h>
#include<signal.h>
void int_func(int sig);
int k;
void int_func(int sig)
{
k=0;
}
main()
{
int chld,shmid;
char *viraddr;
char buffer[BUFSIZ];

shmid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
viraddr=(char *)shmat(shmid,0,0);
signal(SIGUSR1,int_func);
k=1;
while((chld=fork())==-1);
if(chld==0)
{
	while(k==1);
	printf("Your message is:\n%s",viraddr);
	shmdt(viraddr);
	shmdt(shmid,IPC_RMID,0);
	exit(0);
}
else 
{
	while(1)
	{
	printf("Enter some text:");
	fgets(buffer,BUFSIZ,stdin);
	strcat(viraddr,buffer);
	if(strncmp(buffer,"end",3)==0)
		break;
	}
	kill(chld,SIGUSR1);
	exit(0);
}
	
}



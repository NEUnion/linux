#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/shm.h>
main()
{
	int shmid;
	char * viraddr;

	shmid=shmget(1234,BUFSIZ,0666|IPC_CREAT);
	viraddr=(char *)shmat(shmid,0,0);
	
	printf("Your message is:\n%s",viraddr);
	shmat(viraddr);
	shmctl(shmid,IPC_RMID,0);
	exit(0);
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/shm.h>
main()
{
	int shmid;
	char *viraddr;
        char buffer[BUFSIZ];
	shmid=shmget(1234,BUFSIZ,0666|IPC_CREAT);
	viraddr=(char*)shmat(shmid,0,0);

	while (1)
	{
           puts("Enter some text:");	
           fgets(buffer,BUFSIZ,stdin);
	   strcat(viraddr,buffer);
	   if (strncmp(buffer,"end",3)==0) 
		   break;
     }
	   shmdt(viraddr);
	exit(0);
}

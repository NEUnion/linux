#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/shm.h>
main ()
{
	int shmid;
	char *viraddr;
	char buffer[BUFSIZ];

	shmid=shmget(1234,BUFSIZ,0666|IPC_CREAT);
	viraddr=(char*)shmat(shmid,0,0);

	while(1)
	{
		puts("Enter some text:");
		fgets(buffer,BUFSIZ,stdin);
		strcat(viraddr,buffer);
		if(strncmp(buffer,"end",3)==0)
			break;
	}
	shmdt(viraddr);
	exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/shm.h>
main ()
{
	int shmid;
	char *viraddr;

	shmid=shmget(1234,BUFSIZ,0666|IPC_CREAT);
	viraddr=(char*)shmat(shmid,0,0);

	printf("Your message is :\n",viraddr);
	shmdt(viraddr);
	shmctl(shmid,IPC_RMID,0);
	exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/shm.h>
main ()
{
	int chld,shmid;
	char *viraddr;
	char buffer[BUFFSIZ];
	
	shmid=shmget(IPC_PRIVATE,BUFSIZ,0666|IPC_CREAT);
	viraddr=(char*)shmat(shmid,0,0);

	while((chld=fork())==-1);
	if(chld==0)
	{
		while(1)
		{
			puts("Enter some text:");
			fgets(buffer,BUFSIZ,stdin);
			strcat(viraddr,bufffer);
			if(strncmp(buffer,"end",3)==0)
				break;
		}
		exit(0);
	}
	else
	{
		wait(0);
		printf("Your message is :\n%s",viraddr);

		shmdt(viraddr);
		shmctl(shmid,IPC_RMID,0);
		exit(0);
	}

}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAXMSG 11

main()
{
	int i,chld1,chld2;
	int arrayid;
	int *array;


	arrayid=shmget(IPC_PRIVATE,sizeof(int)*MAXMSG,IPC_CREAT|0666);
	array=(int *)shmat(arrayid,0,0);

	while((chld1=fork())==-1);
	if(chld1==0)
	{
		sleep(1);
		for(i=0;i<=10;i++)
			printf("Your message is :%d\n",array[i]);
		exit(0);
	}
	else
	{
		while((chld2=fork())==-1);
		if(chld2==0)
		{
			for(i=0;i<=10;i++)
			{
				array[i]=i;
				printf("Your message is :%d\n",array[i]);
			}
			exit(0);
		}
		else
		{
			wait(0);
			wait(0);
			shmdt(array);
			shmctl(arrayid,IPC_RMID,0);
			exit(0);
		}
	}
}

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void int_func(int sig);
int k;

void int_func(int sig)
{
	k=0;
}

main()
{
	int p;

	signal(SIGUSR1,int_func);
	k=1;

	while((p=fork())==-1);
	if(p==0)
	{
		while(k==1);
		putchar('B');
		exit(0);
	}
	else
	{
		putchar('A');
		kill(p,SIGUSR1);
		exit(0);
	}
}
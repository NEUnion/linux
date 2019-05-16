#include <sys/types.h>
#include <linux/sem.h>
#include <linux/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include<stdlib.h>

#define MAXSHM 5

int fullid;
int emptyid;
int mutexid;

int main()
{
	struct sembuf P,V;
	union semun arg;

	int arrayid;
	int getid;
	int *array;
	int *get;

	arrayid=shmget(IPC_PRIVATE,sizeof(int) *MAXSHM,IPC_CREAT|0666);
	getid=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);

	array=(int *)shmat(arrayid,0,0);
	get=(int *)shmat(getid,0,0);
	*get=0;

	fullid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);

	arg.val=0;
	if(semctl(fullid,0,SETVAL,arg)==-1) perror("error");
	arg.val=MAXSHM;
	if(semctl(emptyid,0,SETVAL,arg)==-1) perror("error");
	arg.val=1;
	if(semctl(mutexid,0,SETVAL,arg)==-1) perror("error");

	P.sem_num=0;
	P.sem_op=-1;
	P.sem_flg=SEM_UNDO;
	V.sem_num=0;
	V.sem_op=1;
	V.sem_flg=SEM_UNDO;

	if(fork()==0)
	{
		int i=0;
		int set=0;
		while(i<10)
		{
			semop(emptyid,&P,1);
			semop(mutexid,&P,1);
			array[set%MAXSHM]=i+1;
			printf("Producer put number %d to NO. %d \n",array[set%MAXSHM],set%MAXSHM);
			set++;
			semop(mutexid, &V,1);
			semop(fullid, &V,1);
			i++;
		}
		sleep(3);
		printf("Producer is over\n");
		exit(0);
	}
	else
	{
		if(fork()==0)
		{
			while(1)
			{
				if(*get ==10)
					break;
				semop(fullid, &P,1);
				semop(mutexid, &P,1);
				printf("The ConsumerA get number from NO. %d\n",(*get)%MAXSHM);
				(*get)++;
				semop(mutexid, &V,1);
				semop(emptyid, &V,1);
				sleep(1);
			}
			printf("ConsumerA is over\n");
			exit(0);
		}
		else
		{
			if(fork()==0)
			{
				while(1)
				{
					if(*get ==10)
						break;

					semop(fullid, &P,1);
					semop(mutexid, &P,1);
					printf("The ConsumerB get number from NO. %d\n",(*get)%MAXSHM);
					(*get)++;
					semop(mutexid, &V,1);
					semop(emptyid, &V,1);
					sleep(1);
				}
				printf("ConsumerB is over\n");
				exit(0);
			}
		}
	}
	wait(0);
	wait(0);
	wait(0);
	shmdt(array);
	shmctl(arrayid,IPC_RMID,0);
	shmdt(get);
	shmctl(getid,IPC_RMID,0);
	semctl(emptyid,IPC_RMID,0);
	semctl(fullid,IPC_RMID,0);
	semctl(mutexid,IPC_RMID,0);
	exit(0);

}

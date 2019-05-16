#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<linux/sem.h>
#include<linux/shm.h>

#define MAXSHM 5

int emptyid;
int fullid;
int mutexid;

main()
{
	int chld;

	struct sembuf P,V;
	union semun arg;

	int arrayid;
	int getid;
	int sum1,sum2;


	int *array;
	int *get;
	int *ns1,*ns2;

	arrayid=shmget(IPC_PRIVATE,sizeof(int)*MAXSHM,IPC_CREAT|0666);
	getid=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);
	sum1=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);
	sum2=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666);


	array=(int *)shmat(arrayid,0,0);
	get=(int *)shmat(getid,NULL,0);
	ns1=(int *)shmat(sum1,NULL,0);
	ns2=(int *)shmat(sum2,NULL,0);
	*ns1=0;
	*ns2=0;
	*get=0;

	emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
    fullid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);

	arg.val=0;
	if (semctl(fullid,0,SETVAL,arg)==-1) perror("semctl setval error");
	arg.val=MAXSHM;
	if (semctl(emptyid,0,SETVAL,arg)==-1) perror("semctl setval error");
	arg.val=1;
	if (semctl(mutexid,0,SETVAL,arg)==-1) perror("semctl setval error");



	P.sem_num=0;
	P.sem_op=-1;
	P.sem_flg=SEM_UNDO;
	V.sem_num=0;
	V.sem_op=1;
	V.sem_flg=SEM_UNDO;


	if (fork()==0)
	{
		int i=0;
int set=0;
		//int sum=0;
		while (i<10)
		{
			semop(emptyid,&P,1);
			semop(mutexid,&P,1);
			array[set%MAXSHM]=i+1;
			//sum+=i;
			//printf("Producer put number %d to No. %d\n",array[set%MAXSHM],set%MAXSHM);
			set++;
			semop(mutexid,&V,1);
			semop(fullid,&V,1);
			i++;
		}
		sleep(3);
		//printf("%d\n",sum);
		printf("The sum is :%d\n",(*ns1)+(*ns2));
		printf("Producer is over!\n");

		exit(0);
	}
	else
	{
		if (fork()==0)
		{
			while (1)
			{
				if (*get==10) break;
				semop(fullid,&P,1);
				semop(mutexid,&P,1);
				//printf("The ComsumerA get number from No. %d\n",(*get)%MAXSHM);
				(*ns1)=(*ns1)+(*get);
				printf("%d ",(*get));
				(*get)++;
				semop(mutexid,&V,1);
				semop(emptyid,&V,1);
				sleep(1);
			}
			printf("The odd sum is :%d\n",(*ns1));
			printf("ConsumerA is over!\n");
			exit(0);
		}
		else 
		{
			if (fork()==0) 
			{
				while (1)
				{
					if (*get==10) break;
					semop(fullid,&P,1);
					semop(mutexid,&P,1);
				//	printf("The ComsumerB get number from No. %d\n",(*get)%MAXSHM);
					(*ns2)+=(*get);
					printf("%d ",(*get));
					(*get)++;
					semop(mutexid,&V,1);
					semop(emptyid,&V,1);
					sleep(1);
				}
			printf("The ood sum is :%d\n",(*ns2));
			printf("ConsumerB is over!\n");
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


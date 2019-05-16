#include<stdio.h>
#include<unistd.h>
#include<signal.h>
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
//signal_ex3.c

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
  int i;
  for (i=1;i<6;i++)
  {
    if (fork()==0)
    {
      printf("i=%d pid=%d\n",i,getpid());
      pause();
    }
  }
  printf("sleeping...\n");
  sleep(3);
  printf("exiting...\n");
  if (kill(0,SIGINT)==-1)
  {
    printf("Could not send signal.\n");
  }
  exit(0);
}
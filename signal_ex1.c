#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int ntimes = 0;

int main()
{
  int pid, ppid;
  void p_catch();
  void c_catch();

  /*设置父进程的SIGUSR1 */
  signal(SIGUSR1,p_catch);
  switch(pid=fork())
  {
    case -1:
      printf("fail fork\n");
      exit(1);
    case 0:
      /*   */
      signal(SIGUSR1,c_catch);
      ppid = getppid();
      for (;;)
      {
	sleep(1);
	/*   */
	kill(ppid,SIGUSR1);
	pause();
      }
      break;
    default:
      for (;;)
      {
	pause();
	sleep(1);
	/*    */
	kill(pid,SIGUSR1);
      }
  }
}

void p_catch()
{
  printf("parent process caught the signal %d\n",++ntimes);
}

void c_catch()
{
  printf("child process caught the signal %d\n",++ntimes);
  
}







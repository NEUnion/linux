//signale_ex2.c

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

void sighandler()
{
  printf("caught signal SIGTERM\n");
}

void init_sigaction()
{
  struct sigaction act;
  act.sa_handler = sighandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGVTALRM,&act,NULL);
}

void init_time()
{
  struct itimerval value;
  value.it_value.tv_sec = 1;
  value.it_value.tv_usec = 0;
  value.it_interval = value.it_value;
  setitimer(ITIMER_VIRTUAL,&value,NULL);
}

main()
{
  init_sigaction();
  init_time();
  while(1)
  {
    raise(SIGALRM);
    printf("pid=%5d",getpid());
    sleep(2);
  }
}

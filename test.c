#include<stdio.h>
main()
{
int i,j;
int p;
while((p=fork())==-1);

if(p==0)
{
for(i=0;i<5;i++)
{
printf("I am child.\n");
sleep(1);
}
}
else 
{
for(j=0;j<5;j++)
{
printf("I am parent.\n");
sleep(1);

}

}
}

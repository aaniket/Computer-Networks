#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
int state=1;

union semun
{
	int val;
	int* array;
};

int sem_init(int no_of_sems,int common_init_val)
{
	int sem_id=semget(9360,no_of_sems,IPC_CREAT|0666);
	if(sem_id==-1)
	{
		printf("error");
		return -1;
	}
	union semun tmp;
	tmp.val=common_init_val;
	int i;
	for(i=0;i<no_of_sems;i++)
	{
		semctl(sem_id,i,SETVAL,tmp);
	}
	return sem_id;
}

int sem_init_multiple(int no_of_sems,int* arr)
{
	key_t h=ftok(".",state++);
	int sem_id=semget(h,no_of_sems,IPC_CREAT|0666);
	if(sem_id==-1)
	{
		printf("error");
		return -1;
	}
	union semun tmp;
	tmp.array=arr;
	semctl(sem_id,0,SETALL,tmp);
	return sem_id;
}

void sem_change(int sem_id,int sem_no,int amt)
{
	struct sembuf tmp;
	tmp.sem_flg=0;
	tmp.sem_op=amt;
	tmp.sem_num=sem_no;
	if(semop(sem_id,&tmp,1)==-1)
	{
		printf("error");
		return ;
	}
}

void sem_try_change(int sem_id,int sem_no,int amt)
{
	struct sembuf tmp;
	tmp.sem_flg=IPC_NOWAIT;
	tmp.sem_op=amt;
	tmp.sem_num=sem_no;
	if(semop(sem_id,&tmp,1)==-1)
	{
		printf("error");
		return ;
	}
}
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet.h>
#include "semaphore.c"

#define maxs 105
#define BUFFSIZE 512
#define shmkey 555
#define semkey 666

#define ID 1

int *cnt;
int mutex;

int main(){
	shmid=shmget(shmkey, BUFFSIZE, IPC_CREAT);
	assert(shmid>=0);
	cnt=(int*)shmat(shmid, 0);

	mutex=semget(semkey+ID, 1, IPC_CREAT);

	int a,b;
	scanf("%d %d", &a, &b);
	printf("%d\n", a+b);

	sem_wait(mutex, 0);
	cnt[ID]--;
	sem_signal(mutex, 0);
	return 0;
}
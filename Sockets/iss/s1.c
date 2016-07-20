#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "semaphore.c"
#include <assert.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define maxs 105
#define BUFFSIZE 105
#define shmkey 555
#define semkey 666

#define ID 1

int *cnt;
int mutex;
int shmid;

int main(){
	shmid=shmget(shmkey, BUFFSIZE, IPC_CREAT);
	assert(shmid>=0);
	cnt=(int*)shmat(shmid, NULL, 0);

	mutex=semget(semkey+ID, 1, IPC_CREAT);

	int a,b;
	scanf("%d %d", &a, &b);
	printf("%d", a/b);

	sem_wait(mutex, 0);
	cnt[ID]--;
	sem_signal(mutex, 0);
	return 0;
}
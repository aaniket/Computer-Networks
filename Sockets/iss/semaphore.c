#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

/**
	helper function to operate on semaphore
**/
int sem_op(int sem_id, int index, int val){
	struct sembuf buf;
	buf.sem_num=index;
	buf.sem_flg=0;
	buf.sem_op=val;
	if(semop(sem_id, &buf, 1)==-1){
		printf("semop error\n");
		return 0;
	}
	return 1;
}

/**
	helper function to increment semaphore
**/
int sem_signal(int sem_id, int index){
	if(sem_op(sem_id, index, 1)==0){
		printf("semaphore signal error\n");
		return 0;
	}
	return 1;
}

/**
	helper function for decrementing semaphore
**/
int sem_wait(int sem_id, int index){
	if(sem_op(sem_id, index, -1)==0){
		printf("semaphore signal error\n");
		return 0;
	}
	return 1;
}

union semun{
		int val, *array;
}u;

/**
	helper function to initialise semaphore
**/
int sem_init(int key, int num, int value){
	int sem_id;
	if((sem_id=semget(key, num, IPC_CREAT|0666))==-1){
		printf("semaphore creation error\n");
		return 0;
	}

	u.val=value;
	if((semctl(sem_id, 0, SETVAL, u))==-1){
		printf("semctl error\n");
		return 0;
	}
	return sem_id;
}


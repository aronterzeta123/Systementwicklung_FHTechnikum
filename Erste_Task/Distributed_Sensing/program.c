#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

typedef struct SensP{
        int press;
	long int mtype;
}SensP;
typedef struct SensH{
        int humid;
	long int mtype;
}SensH;
typedef struct SensT{
        int temp;
	long int mtype;
}SensT;
int main(int argc, char *argv[]){

key_t SomeKey;
SomeKey = ftok("/etc/hostname", 'b');
int msqid=msgget(SomeKey, IPC_CREAT | 0666);
struct SensH humidity;
humidity.mtype=3;
struct SensT tmp;
tmp.mtype=1;
struct SensP press;
press.mtype=2;
		printf("kok");
if (msqid >= 0) {
		while (1) {
			if (msgrcv (msqid, &humidity, 16,3, 0) == -1) {
				perror ("msgrcv");
			} else {
				printf ("Received Message: %s\n",humidity.humid );
			}
		}
	} else {
		perror ("msgget");
	}

return 0;


}

#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
struct my_msgbuf {
   long mtype;
   char mtext[200];
};
typedef struct SensT {
	int minTemp;
	int temp;
	int maxTemp;
	long int mtype;
} SensT;
typedef struct SensP {
	int minPress;
	int press;
	int maxPress;
	long int mtype;
} SensP;
typedef struct SensH {
	int minHumid;
	int humid;
	int maxHumid;
	long int mtype;
} SensH;
void *tempSens (void *ptr)
{
	srand (time (NULL));
	// void pointer kann auf alles zeigen
	struct SensT* tempsensor = (struct SensT*) ptr;
	tempsensor->minTemp = 233;
	tempsensor->maxTemp = 313;
	tempsensor->temp = (rand() % (tempsensor->maxTemp - tempsensor->minTemp + 1)) + tempsensor->minTemp;
	tempsensor->mtype = 1;
}
void *pressSens (void *ptr)
{
	srand (time (NULL));
	// void pointer kann auf alles zeigen
	struct SensP* presssensor = (struct SensP*) ptr;
	presssensor->minPress = 96000;
	presssensor->maxPress = 140000;
	presssensor->press = (rand() % (presssensor->maxPress - presssensor->minPress + 1)) + presssensor->minPress;
	presssensor->mtype = 2;
}
void *humiditySens (void *ptr)
{
	srand (time (NULL));
	struct SensH* humiditysensor = (struct SensH*) ptr;
	humiditysensor->minHumid = 10;
	humiditysensor->maxHumid = 100;
	humiditysensor->humid = (rand() % (humiditysensor->maxHumid - humiditysensor->minHumid + 1)) + humiditysensor->minHumid;
	humiditysensor->mtype = 3;
}

int main (int argc, char *argv[])
{
	key_t SomeKey;
	SomeKey = ftok ("/etc/hostname", 'b');
	int msqid = msgget (SomeKey, IPC_CREAT | 0666);
	pid_t pid1, pid2, pid3, pid4, pid5, pid6;
	int c = 0;
	while ( (c = getopt (argc, argv, "HTP")) != -1)
		switch (c) {
		case 'H':
			pid1 = fork();
			struct my_msgbuf buf;
			struct SensH* humidsensor = malloc (sizeof (struct SensH));
			if (pid1 > 0) {
				while(1){
				if (msqid >= 0) {
					humiditySens (humidsensor);
					sprintf(buf.mtext,"%d",humidsensor->humid);
					//strcpy(buf.mtext,humidsensor->humid);
					buf.mtype=3;
					if (msgsnd (msqid, &buf, strlen(buf.mtext), 0) == -1) {
						perror ("msgsnd");
					} else {
						printf ("Data sent: %d\n", humidsensor->humid);
					}

				}

				sleep(10);
			}
			}
			break;


		case 'T':
			pid2 = fork();
			struct my_msgbuf buf1;
			struct SensT* tempsensor = malloc (sizeof (struct SensT));
			if (pid2 > 0) {
				while(1){
				if (msqid >= 0) {
					tempSens (tempsensor);
					sprintf(buf1.mtext,"%d",tempsensor->temp);
					buf1.mtype=1;
					if (msgsnd (msqid, &buf1, strlen(buf1.mtext), 0) == -1) {
						perror ("msgsnd");
					} else {
						printf ("Data sent: %d\n", tempsensor->temp);
					}
				}
				sleep(10);
			}
			}
			break;
		case 'P':
			pid3 = fork();
			struct my_msgbuf buf2;
			struct SensP* presssensor = malloc (sizeof (struct SensP));
			if (pid3 > 0) {
				while(1){
				if (msqid >= 0) {
					pressSens (presssensor);
					sprintf(buf2.mtext,"%d",presssensor->press);
					buf2.mtype=2;
					if (msgsnd (msqid, &buf2, strlen(buf2.mtext), 0) == -1) {
						perror ("msgsnd");
					} else {
						printf ("Data sent: %d\n", presssensor->press);
					}
				}
				sleep(10);
			}
			}
			break;
		default:
			printf ("You should give an argument: -H, -T, -P");
			break;
		}
	return 0;
}

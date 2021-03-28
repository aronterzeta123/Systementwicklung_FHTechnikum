#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

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
			struct SensH* humidsensor = malloc (sizeof (struct SensH));
			if (pid1 > 0) {
				//while(1){
				if (msqid >= 0) {
					humiditySens (humidsensor);
					printf ("%d", humidsensor->humid);
					if (msgsnd (msqid, humidsensor, 16, 0) == -1) {
						perror ("msgsnd");
					} else {
// MHOR
// CORRECT
						printf ("Data sent: %d\n", humidsensor->humid);
// WRONG
//						printf ("Data sent: %s\n", humidsensor->humid);
					}

				}

				//sleep(10);

			}
			break;


		case 'T':
			pid2 = fork();
			struct SensT* tempsensor = malloc (sizeof (struct SensT));
			if (pid2 > 0) {
				//while(1){
				if (msqid >= 0) {
					tempSens (tempsensor);
					printf ("%d", tempsensor->temp);
					if (msgsnd (msqid, tempsensor, 16, 0) == -1) {
						perror ("msgsnd");
					} else {
// MHOR
// CORRECT
						printf ("Data sent: %d\n", tempsensor->temp);
// WRONG
//						printf ("Data sent: %s\n", tempsensor->temp);
					}
				}
				//sleep(10);
			}
			break;
		case 'P':
			pid3 = fork();
			struct SensP* presssensor = malloc (sizeof (struct SensP));
			if (pid3 > 0) {
				//while(1){
				if (msqid >= 0) {
					pressSens (presssensor);
					printf ("%d", presssensor->press);
					if (msgsnd (msqid, presssensor, 16, 0) == -1) {
						perror ("msgsnd");
					} else {
// MHOR
// CORRECT
						printf ("Data sent: %d\n", presssensor->press);
// WRONG
//						printf ("Data sent: %s\n", presssensor->press);
					}
				}
				//sleep(10);
			}
			break;
		case 'H' & 'T':
			printf ("dwadwa");
			break;
		default:
			printf ("You should give an argument: -H, -T, -P");
			break;
		}
	return 0;
}

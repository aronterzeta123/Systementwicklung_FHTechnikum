#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

typedef struct SensT{
        int minTemp;
        double temp;
        int maxTemp;
}SensT;
typedef struct SensP{
        int minPress;
        double press;
        int maxPress;
}SensP;
typedef struct SensH{
        int minHumid;
        double humid;
        int maxHumid;
}SensH;
void *tempSens(void *ptr){
	srand(time(NULL));
        // void pointer kann auf alles zeigen
        struct SensT* tempsensor=(struct SensT*) ptr;
        tempsensor->minTemp=233;
	tempsensor->maxTemp=313;
	tempsensor->temp=(rand() % (tempsensor->maxTemp - tempsensor->minTemp+1))+tempsensor->minTemp;
}
 
int main(int argc, char *argv[]) {
struct SensT sensor1;

key_t SomeKey;
SomeKey = ftok(".", 'x');
int msqid=msgget(SomeKey, IPC_CREAT);

pid_t pid;
pid=fork();      //Diese beide Zeile, soll ich  unten bei if-Machen ?
	if( strcmp(argv[1], "-H") == 0 || strcmp(argv[1], "-T") == 0){
		if(pid >= 0){ //new, child process
			if(msqid >=0){
				tempSens(&sensor1);
				printf("%f",sensor1.temp);
				}
			else{
				printf("msgget error");
				}
		}
		else{
			printf("Child process not created");
		}
	}else{
	     printf("dkwoadka");
	}

return 0;
}
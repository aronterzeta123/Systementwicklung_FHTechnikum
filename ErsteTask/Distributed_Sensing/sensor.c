#include  <sys/types.h>
#include  <sys/ipc.h>
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






int main(int argc, char *argv[]) {
key_t SomeKey;
SomeKey = ftok(".", 'x');
int msqid=msgget(SomeKey, IPC_CREAT);
if(msqid >=0){
	
}
else{
	printf("msgget error");
}
return 0;
}

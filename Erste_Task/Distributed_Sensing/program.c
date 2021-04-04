#include <sys/types.h>
#include  <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
struct my_msgbuf {
   long mtype;
   char mtext[200];
};
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

static int msqid=0;

void cntrl_c_handler (int dummy)
{
	msgctl(msqid, IPC_RMID, NULL);
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]){

key_t SomeKey;
SomeKey = ftok("/etc/hostname", 'b');
msqid=msgget(SomeKey, IPC_CREAT | 0666);
int ca=0;
struct my_msgbuf buf;
struct my_msgbuf buf1;
struct my_msgbuf buf2;
double meantempcelsius=0.0;
double meantempfahrenheit=0.0;
double meanatm=0.0;
int counter=0;
int counter1=0;
int counter2=0;
int sum2=0;
int sum1=0;
int sum=0;
pid_t pid1,pid2,pid3;
if (msqid >= 0) {
      while ( (ca = getopt (argc, argv, "cfa")) != -1)
                switch (ca) {
                case 'c':
	pid1=fork();
	if(pid1>0){
	FILE *fp;
	fp = fopen("sensorWerte.txt", "w+") ;
	if ( fp == NULL )
    {
        printf( "Could not open file sensorWerte.txt" ) ;
        return 1;
    }
	for(;;){
	if (msgrcv(msqid, &buf, sizeof(buf.mtext), 1, 0) == -1) {
         perror("msgrcv");
         exit(1);
         }	
		sum=sum+atoi(buf.mtext);
		counter++;
		if(counter%10==0){meantempcelsius=sum/10-273.15;printf("Mean Value of temp for 10Values in Celcius is: %f\n",meantempcelsius);fprintf(fp,"Temperatur in Celsius: %f \n",meantempcelsius);fclose(fp);sum=0;}
		signal(SIGINT,cntrl_c_handler);
		}
	}	
		break;
                case 'f':
	pid2=fork();
	if(pid2>0){
	FILE *fp1;
	fp1 = fopen("sensorWerte.txt", "w+") ;
	if ( fp1 == NULL )
    {
        printf( "Could not open file sensorWerte.txt" ) ;
        return 1;
    }
	for(;;){
	if (msgrcv(msqid, &buf2, sizeof(buf2.mtext), 1, 0) == -1) {
         perror("msgrcv");
         exit(1);
         }	
		sum1=sum1+atoi(buf2.mtext);
		counter1++;
		if(counter1%10==0){meantempfahrenheit=(sum1/10-273.15)*9/5+32;printf("Mean Value of temp for 10Values in Fahrenheit is: %f\n",meantempfahrenheit);fprintf(fp1,"Temperatur in Fahrenheit: %f \n",meantempfahrenheit);fclose(fp1);sum1=0;}
		signal(SIGINT,cntrl_c_handler);
		}
	}
	break;
                case 'a':
	pid3=fork();
	if(pid3>0){
	FILE *fp2;
	fp2 = fopen("sensorWerte.txt", "w+") ;
	if ( fp2 == NULL )
    {
        printf( "Could not open file sensorWerte.txt" ) ;
        return 1;
    }
	for(;;){
	if (msgrcv(msqid, &buf1, sizeof(buf1.mtext), 2, 0) == -1) {
         perror("msgrcv");
         exit(1);
         }	
		sum2=sum2+atoi(buf1.mtext);
		counter2++;
		if(counter2%10==0){meanatm=(sum2/10)/101325;printf("Mean Value of Pressure for 10Values in ATM is: %f\n",meanatm);fprintf(fp2,"Pressure in ATM: %f \n",meanatm);fclose(fp2);sum2=0;}
		signal(SIGINT,cntrl_c_handler);
		}
		}
		break;
	}	
	} else {
		perror ("msgget");
	}
	
return 0;

}

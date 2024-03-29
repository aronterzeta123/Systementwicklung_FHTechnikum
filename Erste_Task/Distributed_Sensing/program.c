#include <math.h>
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
#include <stdbool.h>
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
bool cparameter=false;
bool fparameter=false;
bool aparameter=false;
static int arguments=0;
static FILE *fp,*celsiussvg,*fahrenheitsvg,*presssvg = NULL;
void cntrl_c_handler ()
{
	msgctl(msqid, IPC_RMID, NULL);
	if(cparameter && !fparameter){
	fprintf(celsiussvg,"</svg> ");
	fclose(celsiussvg);
	}
	if(fparameter && !cparameter){	
	fprintf(fahrenheitsvg,"</svg> ");	
	fclose(fahrenheitsvg);
	}
	if(aparameter){
	fprintf(presssvg,"</svg> ");	
	fclose(presssvg);
	}
	fclose(fp);
}


int main(int argc, char *argv[]){

key_t SomeKey;
signal(SIGINT,cntrl_c_handler);
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
celsiussvg= fopen("tempWerteCelsius.svg", "w+");
fahrenheitsvg= fopen("tempWerteFahrenheit.svg", "w+");
presssvg= fopen("pressWerte.svg", "w+");
if (msqid >= 0) {
	fp = fopen("sensorWerte.txt", "w+");
	if ( fp == NULL)
    {
        printf( "Could not open file sensorWerte.txt" ) ;
        return 1;
    }
      while ( (ca = getopt (argc, argv, "cfa")) != -1)
                switch (ca) {
                case 'c':
	cparameter=true;
	arguments++;
	if ( celsiussvg == NULL)
    {
        printf( "Could not open file tempWerteCelsius.svg" ) ;
        return 1;
    }
		break;
		case 'f':
	fparameter=true;
	arguments++;
	if ( fahrenheitsvg == NULL)
    {
        printf( "Could not open file tempWerteFahrenheit.svg" ) ;
        return 1;
    }
		break;
		case 'a':
	aparameter=true;
	arguments++;
	if ( presssvg == NULL)
    {
        printf( "Could not open file pressWerte.svg" ) ;
        return 1;
    }
		break;
	}
	if(cparameter && !fparameter){
	pid1=fork();
	if(pid1>0){
        int i=0;
	fprintf(celsiussvg,"<?xml version=\"1.0\" encoding=\"UTF-8\"?> <svg width=\"700px\" height=\"400px\" viewBox=\"0 0 1200 400\">");
	for(;;){
	if (msgrcv(msqid, &buf, sizeof(buf.mtext), 1, 0) == -1) {
         printf("Message Queue deleted from sensor Task");
         exit(1);
	}
		sum=sum+atoi(buf.mtext);
		counter++;
		if(counter%10==0)
		{
		meantempcelsius=sum/10-273.15;printf("Mean Value of temp for 10Values in Celcius is: %f\n",meantempcelsius);
		fprintf(fp,"Temperatur in Celsius: %f \n",meantempcelsius);sum=0;
		fprintf(celsiussvg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%f\" stroke=\"blue\" stroke-width=\"3px\" />",100+(i*10),100,100+(i*10),fabs(meantempcelsius));
		i++;
		}
		}
	}
	}
	if(fparameter && !cparameter){
	pid2=fork();
	if(pid2>0){
	int i=0;
	fprintf(fahrenheitsvg,"<?xml version=\"1.0\" encoding=\"UTF-8\"?> <svg width=\"700px\" height=\"400px\" viewBox=\"0 0 1200 400\">");
	for(;;){
	if (msgrcv(msqid, &buf2, sizeof(buf2.mtext), 1, 0) == -1) {
         printf("Message Queue deleted from sensor Task");
         exit(1);
         }	
		sum1=sum1+atoi(buf2.mtext);
		counter1++;
		if(counter1%10==0)
		{
		meantempfahrenheit=(sum1/10-273.15)*9/5+32;
		printf("Mean Value of temp for 10Values in Fahrenheit is: %f\n",meantempfahrenheit);
		fprintf(fp,"Temperatur in Fahrenheit: %f \n",meantempfahrenheit);sum1=0;
		fprintf(fahrenheitsvg,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%f\" stroke=\"blue\" stroke-width=\"3px\" />",100+(i*10),100,100+(i*10),fabs(meantempfahrenheit));
                i++;
		
		}
		}
	}
	}
	if(aparameter){
	pid3=fork();
	if(pid3>0){
	int i=0;
	fprintf(presssvg,"<?xml version=\"1.0\" encoding=\"UTF-8\"?> <svg width=\"700px\" height=\"400px\" viewBox=\"0 0 1200 400\">");
	for(;;){
	if (msgrcv(msqid, &buf1, sizeof(buf1.mtext), 2, 0) == -1) {
         printf("Message Queue deleted from sensor Task");
         exit(1);
         }	
		sum2=sum2+atoi(buf1.mtext);
		counter2++;
		if(counter2%10==0)
		{
		meanatm=(sum2/10)/101325;
		printf("Mean Value of Pressure for 10Values in ATM is: %f\n",meanatm);
		fprintf(fp,"Pressure in ATM: %f \n",meanatm);sum2=0;
		fprintf(presssvg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%f\" stroke=\"blue\" stroke-width=\"3px\" />",100+(i*10),100,100+(i*10),fabs(meanatm));
                i++;
		}
		}
		}
	}
	if(cparameter && fparameter){
	printf("Not allowed\n");
	exit(1);
	}
	} else {
		perror ("msgget");
	}
return 0;

}

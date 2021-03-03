#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void *tempSens(void *ptr){
	// void pointer kann auf alles zeigen
	struct SensT* tempsensor=(struct SensT*) ptr;
	do{
		if(zh->start==1){
			zh->start=0;
			zh->calc--;
	}}while(zh->stop!=5);
}
void *aron1(void *ptr){
	// void pointer kann auf alles zeigen
	//char *c=&m.b;
	//c=(char)ptr;
	struct st_sto_calc* zh=(struct st_sto_calc*) ptr;
	//printf("%d\t",zh->a);
	//printf("%c\t",zh->b);
	//printf("%f\t",zh->c);
	do{
		if(zh->start==1){
			zh->start=0;
			zh->calc++;
	}}while(zh->stop!=7);
}
int main(int argc, char** argv){
	srand(time(NULL));
	int r=rand()%20;
	pthread_t t1,t2;
	//struct kudiun k;
	//k.a=10;
	//k.b='c';
	//k.c=15.34129;
	//struct kudiun a;
	//a.a=20;
	//a.b='v';
	//a.c=1521.412321;
	struct st_sto_calc k;
	int opt;
	while((opt=getopt(argc,argv,":if:start"))!=-1){
		switch(opt){
			case 's':
			printf("");
		}
	}
	for(;optind<argc;optind++){
	k.start=atoi(argv[optind]);}
	//k.start=0;
	k.stop=0;
	k.calc=0;
	//struct st_sto_calc a;
	int ret=pthread_create(&t1,NULL,aron,(void*)&k);
	//int ret2=pthread_create(&t2,NULL,aron,(void*)&a);
	int ret2=pthread_create(&t2,NULL,aron1,(void*)&k);
	if(ret){
		return 0;
	}
	if(ret2){
		return 0;
	}
	for(int i=0;i<r;i++){
		/*k.start=0;
		k.stop++;
		k.calc=k.calc+i;
		k.calc=k.calc-i;
		sleep(1);
		printf("%d\n %d\n %d\n", k.start,k.stop,k.calc);*/
		//t2.sleep(7);
		//a.start=0;
		//a.stop++;
		//a.calc+=a.calc;
		k.start=1;
		//k.stop++;
		sleep(1);
		printf("%d\n %d\n %d\n",k.start,k.stop,k.calc);
		//printf("%d\n %d\n %d\n", k.start,k.stop,k.calc);
	}

	k.stop=5;
	sleep(3);
	k.stop=7;
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
	// kur e kompilon duhet me e shkru ne fund te gcc -lpthread
}

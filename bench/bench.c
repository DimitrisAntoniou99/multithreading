#include "bench.h"
#include <pthread.h>

//---------------PROSTHETOS CODE ----------------------
//Topothethsh prototypwn twn synarthsewn pou vriskontai sthn kiwi.c gia na mporesoume na tis kalesoume mesa sta nhmata
void *readwrite_test(void *arg);
void *_write_test(void *arg);
void *_read_test(void *arg);
//--------------END OF PROSTHETOS CODE-----------------
void _random_key(char *key,int length) {
	int i;
	char salt[36]= "abcdefghijklmnopqrstuvwxyz0123456789";

	for (i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}

void _print_header(int count)
{
	double index_size = (double)((double)(KSIZE + 8 + 1) * count) / 1048576.0;
	double data_size = (double)((double)(VSIZE + 4) * count) / 1048576.0;

	printf("Keys:\t\t%d bytes each\n", 
			KSIZE);
	printf("Values: \t%d bytes each\n", 
			VSIZE);
	printf("Entries:\t%d\n", 
			count);
	printf("IndexSize:\t%.1f MB (estimated)\n",
			index_size);
	printf("DataSize:\t%.1f MB (estimated)\n",
			data_size);

	printf(LINE1);
}

void _print_environment()
{
	time_t now = time(NULL);

	printf("Date:\t\t%s", 
			(char*)ctime(&now));

	int num_cpus = 0;
	char cpu_type[256] = {0};
	char cache_size[256] = {0};

	FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
	if (cpuinfo) {
		char line[1024] = {0};
		while (fgets(line, sizeof(line), cpuinfo) != NULL) {
			const char* sep = strchr(line, ':');
			if (sep == NULL || strlen(sep) < 10)
				continue;

			char key[1024] = {0};
			char val[1024] = {0};
			strncpy(key, line, sep-1-line);
			strncpy(val, sep+1, strlen(sep)-1);
			if (strcmp("model name", key) == 0) {
				num_cpus++;
				strcpy(cpu_type, val);
			}
			else if (strcmp("cache size", key) == 0)
				strncpy(cache_size, val + 1, strlen(val) - 1);	
		}

		fclose(cpuinfo);
		printf("CPU:\t\t%d * %s", 
				num_cpus, 
				cpu_type);

		printf("CPUCache:\t%s\n", 
				cache_size);
	}
}

int main(int argc,char** argv)
{
	long int count;
	struct arguments thread_args;		//antikeimeno tou struct arguments pou krataei plhroforia gia kathe nhma (dhladh poses diergasies ektelei to kathe nhma ,to r kai to pososto)
	long int threads,threads1,threads2;	//arithmos gia ta threads pou dinoume sth grammh antolwn antistoixa gia readwrite,write,read	
	int i;
	struct th writethreads;			//2 antikeimena gia ta dedwmena pou krataei to programma mas otan kanei write kai read(sthn 85)
	struct th readthreads;	
	srand(time(NULL));
	if (argc < 3) {
		fprintf(stderr,"Usage: db-bench <write | read | readwrite > <count>\n");
		exit(1);
	}
//Eisagwgh neas leitourgias write me polynhmatismo	
	if (strcmp(argv[1], "write") == 0) {
		int i;
		writethreads.a = 0; 		//r		
		threads1 = atoi(argv[3]);	//to 4o input einai ta nhmata
		count = atoi(argv[2]);		//to 3o input einai oi diergasies
		_print_header(count);
		_print_environment();
		if (argc == 5)			//allaxame to 4 se 5 giati efoson valame allo ena input sth grammh entolwn tha mas evgaze gia panta random keys(apo thn kiwi) efoson h timh tha htan 1 
			writethreads.a = 1;
		pthread_t id[threads1];		//ftiaxoume threads analogws me to input sth grammh entolwn
		writethreads.counter = count/threads1;		//vazoume sto antikeimeno writethreads sto counter ton arithmo diergasiwn pou kanei kathe nhma dld count /threads an exoume dld 1000 counts kai 10 nhmata 1000/10 krataei to writethreads

		for(i = 0;i < threads1;i++){			//dhmhourgia twn threads kai to trexoume me for efoson einai polla kai oxi ena 
			pthread_create(&id[i],NULL,_write_test,(void *) &writethreads);		//trexoume ta nhmata na kaloun thn _write_test
		}	
		for(i = 0;i < threads1;i++){			
			pthread_join(id[i],NULL);
		}
//Eisagwgh neas leitourgias read me polynhmatismo
	} else if (strcmp(argv[1], "read") == 0) {	//h ypoloihsh ths else if ths read einai paromoia me thn if ths write me th monh diafora oti ta nhmata kaloun thn _read_test
		int i;
		readthreads.a=0;		//r
		threads2 = atoi(argv[3]);	//to 4o input einai ta nhmata	
		count = atoi(argv[2]);		//to 3o input einai oi diergasies
		
		_print_header(count);
		_print_environment();
		if (argc == 5)			//allaxame to 4 se 5 giati efoson valame allo ena input sth grammh entolwn tha mas evgaze gia panta random keys(apo thn kiwi) efoson h timh tha htan 1 
			readthreads.a = 1;
		pthread_t id1[threads2];	
		readthreads.counter = count/threads2;		
		for(i = 0;i < threads2;i++){
			pthread_create(&id1[i],NULL,_read_test,(void *) &readthreads);		
		}
		for(i = 0;i < threads2;i++){
			pthread_join(id1[i],NULL);
		}
	} 
	
//--------------------------------------PROSTHETOS CODE-------------------------------------------
//Eisagwgh neas leitourgia readwrite opou ektelountai parallhla put kai get me vash to input sth grammh entolwn argv[1]
	else if(strcmp(argv[1], "readwrite") == 0){	//ftiaxame thn epilogh na mporei o xrthsths na kanei read kai write taftoxrona
		thread_args.a = 0; //r				
		count= atoi(argv[2]); //count
		_print_header(count);
		_print_environment();
		if (argc == 6)				//valame 6 wste otan dwsoume kiallo ena input meta to percent na kanei random key search sthn kiwi
			thread_args.a = 1;
		if(atoi(argv[4]) > 100){		//an to pososto einai panw apo 100% na petaei sxetiko error kai exodo apo to programma
			printf("Wrong input value for percent!");
			exit(1);
		}
		threads = atoi(argv[3]); //threadds
		thread_args.percent = atoi(argv[4]); //percent
		pthread_t tid[threads];			//ftiaxoume threads analogws me to input sth grammh entolwn
		thread_args.counter = count/threads;//katanomh		
		for(i = 0;i < threads;i++){		//dhmhourgia twn threads kai to trexoume me for efoson einai polla kai oxi ena 
			printf("--------------------------------------------------------------");			
			pthread_create(&tid[i],NULL,readwrite_test,(void *) &thread_args);		//kanoume ta nhmata create kai kaloun thn readwrite_test me input afto pou exoume valei sto counter kai a tou antikeimenou thread_args tou struct argmunets
		}
		for(i=0;i<threads;i++){
			pthread_join(tid[i],NULL);
		}
		printf("all threads finished\n");

	}
//------------------------------------END OF PROSTHETOS CODE--------------------------------------	 

	else {
		fprintf(stderr,"Usage: db-bench <write | read | readwrite > <count> <random>\n");
		exit(1);
	}

	return 1;
}

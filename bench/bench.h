#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define KSIZE (16)
#define VSIZE (1000)

#define LINE "+-----------------------------+----------------+------------------------------+-------------------+\n"
#define LINE1 "---------------------------------------------------------------------------------------------------\n"

//-------------------------------------PROSTHETOS CODE------------------------------------------
//strucks poy kratane plhroforia gia ta nhmata
struct arguments{		//kratame plhroforia gia ta threads pou tha th xrhsimopoihsoyme sthn readwrite_test
	
	long int counter;
	int a; 
	double percent;
};
struct th{			//kratame plhroforia gia ta threads pou tha th xrhsimopoihsoyme stis _read_test kai _write_test
	long int counter;
	int a;
};


//-------------------------------------END OFF PROSTHETOS CODE----------------------------------



long long get_ustime_sec(void);
void _random_key(char *key,int length);

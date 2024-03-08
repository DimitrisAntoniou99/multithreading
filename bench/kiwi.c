#include <string.h>
#include "../engine/db.h"
#include "../engine/variant.h"
#include "bench.h"
#include <pthread.h>
#define DATAS ("testdb")

double cost; //global metavlhth
//Polynhmatikh ylopoihsh ths _write_test
void *_write_test(void *arg)
{
	int i;
	long long start,end;
	Variant sk, sv;
	DB* db;
	struct th *f = (struct th *) arg;//antikeimeno th(f) pou pernei ta dedwmena apo ta nhmata ths main sthn bench.c
	char key[KSIZE + 1];
	char val[VSIZE + 1];
	char sbuf[1024];

	memset(key, 0, KSIZE + 1);
	memset(val, 0, VSIZE + 1);
	memset(sbuf, 0, 1024);

	db = db_open(DATAS);

	start = get_ustime_sec();
	for (i = 0; i < (f->counter); i++) {  //to f exei access sto struct pou phre (th) kai me to f->counter exoume to counter tou nhmatos apo thn bench.c
		if (f->a)				//f->a einai to r pou exei lavei apo to nhma ths main apo thn bench.c
			_random_key(key, KSIZE);
		else
			snprintf(key, KSIZE, "key-%d", i);
		fprintf(stderr, "%d adding %s\n", i, key);
		snprintf(val, VSIZE, "val-%d", i);

		sk.length = KSIZE;
		sk.mem = key;
		sv.length = VSIZE;
		sv.mem = val;

		db_add(db, &sk, &sv);
		if ((i % 10000) == 0) {
			fprintf(stderr,"random write finished %d ops%30s\r", 
					i, 
					"");

			fflush(stderr);
		}
	}

	db_close(db);

	end = get_ustime_sec();
	cost = end -start;

	printf(LINE);
	printf("|Random-Write	(done:%ld): %.6f sec/op; %.1f writes/sec(estimated); cost:%.3f(sec);\n"
		,f->counter, (double)(cost /f->counter)				//kaname tis aparaithtes antikatastaseis twn dedwmenwn opou count valame to f->counter 
		,(double)(f->counter / cost)
		,cost);	
	return 0;						//prosthikh dikh mas gt den termatize alliws giati o compiler den evlepe an termatizei h synarthsh
}
//Polynhmatikh ylopoihsh ths  _read_test 
void *_read_test(void *arg)
{
	struct th *g = (struct th *) arg;	//allo antikeimeno th (g) pou pernei ta dedwmena apo ta nhmata ths main sthn bench.c
	int i;
	int ret;
	int found = 0;
	long long start,end;
	Variant sk;
	Variant sv;
	DB* db;
	char key[KSIZE + 1];

	db = db_open(DATAS);
	start = get_ustime_sec();
	for (i = 0; i < g->counter; i++) {	//to g exei access sto struct pou phre (th) kai me to g->counter exoume to counter tou nhmatos apo thn bench.c
		memset(key, 0, KSIZE + 1);

		/* if you want to test random write, use the following */
		if (g->a)			//g->a einai to r pou exei lavei apo to nhma ths main apo thn bench.c
			_random_key(key, KSIZE);
		else
			snprintf(key, KSIZE, "key-%d", i);
		fprintf(stderr, "%d searching %s\n", i, key);
		sk.length = KSIZE;
		sk.mem = key;
		ret = db_get(db, &sk, &sv);
		if (ret) {
			//db_free_data(sv.mem);
			found++;
		} else {
			INFO("not found key#%s", 
					sk.mem);
    	}

		if ((i % 10000) == 0) {
			fprintf(stderr,"random read finished %d ops%30s\r", 
					i, 
					"");

			fflush(stderr);
		}
	}

	db_close(db);

	end = get_ustime_sec();
	cost = end - start;
	printf(LINE);
	printf("|Random-Read	(done:%ld, found:%d): %.6f sec/op; %.1f reads /sec(estimated); cost:%.3f(sec)\n",
		g->counter, found,
		(double)(cost / g->counter),		//kaname tis aparaithtes antikatastaseis twn dedwmenwn opou count valame to g->counter 
		(double)(g->counter / cost),
		cost);
	return 0;					//prosthikh dikh mas gt den termatize alliws giati o compiler den evlepe an termatizei h synarthsh
}

//------------------------------------PROSTHETOS CODE----------------------------------------------------
//Synarthsh readwrite gia na kanei taftoxrona kai read kai writes 
void *readwrite_test(void *arg){
	long long start,end;
	struct arguments *d = (struct arguments *) arg;		//allo antikeimeno arguments (d) pou pernei ta dedwmena apo ta nhmata ths main sthn bench.c
	int i,ret;						//exoume valei ola ta dedwmena apo tis hdh yparxouses read_test kai write_test gia na ta xrhsimopoihsoume wste na kanei kai reads kai writes
	int countread,countwrite;	
	Variant sk, sv;	
	DB* db;
	int found = 0;
	char key[KSIZE + 1];
	char val[VSIZE + 1];
	char sbuf[1024];
	memset(key, 0, KSIZE + 1);
	memset(val, 0, VSIZE + 1);
	memset(sbuf, 0, 1024);
	
	double x;						
	x = (d->percent)*0.01;					//to pososto pou dinoume sth grammh entolws kata thn ektelesh tou programmatos exoume valei san default na einai to pososto twn read pou kasnei kathe thread
	db = db_open(DATAS);					//dhladh an dwsoume 40 gia pososto to kathe nhma tha kanei 40% reads kai 60% writes 
	countread = (d->counter)*x;				//to x metatrepei to input pososto se arithmo apo to 0 mexri to 1 kai ftiaxnoume to countread pou einai ta reads kai to countwrite pou einai o arithmos twn writes
	countwrite = (d->counter) - countread;	
	start = get_ustime_sec();	
	
		

	for (i = 0; i < countread; i++) {			//edw 3ekinaei kai kanei reads analoga me to posa countreads exoume
		memset(key, 0, KSIZE + 1);			//h for einai idia me thn for sthn read apla exoume allaxei to posa read tha kanei
		
		
		if (d->a)
			_random_key(key, KSIZE);
		else
			snprintf(key, KSIZE, "key-%d", i);
		fprintf(stderr, "%d searching %s\n", i, key);
		sk.length = KSIZE;
		sk.mem = key;
		ret = db_get(db, &sk, &sv);
		if (ret) {
			//db_free_data(sv.mem);
			found++;
		} else {
			INFO("not found key#%s", 
					sk.mem);
    	}

		if ((i % 10000) == 0) {
			fprintf(stderr,"random read finished %d ops%30s\r", 
					i, 
					"");

			fflush(stderr);
		}
	}
		
	for (i = 0; i < countwrite; i++) {			//kai edw sunexizei me ta writes analoga me ta countwrites
								//h for einai idia me thn for sthn write apla exoume allaxei to posa write tha kanei
		if (d->a)					
			_random_key(key, KSIZE);
		else
			snprintf(key, KSIZE, "key-%d", i);
		fprintf(stderr, "%d adding %s\n", i, key);
		snprintf(val, VSIZE, "val-%d", i);

		sk.length = KSIZE;
		sk.mem = key;
		sv.length = VSIZE;
		sv.mem = val;

		db_add(db, &sk, &sv);
		if ((i % 10000) == 0) {
			fprintf(stderr,"random write finished %d ops%30s\r", 
					i, 
					"");

			fflush(stderr);
		}
	}
	
	
	
	db_close(db);
	end = get_ustime_sec();
	cost = end -start;

	printf(LINE);
	printf("|Random-ReadWrite	(done:%ld): %.6f sec/op; %.1f readwrites/sec(estimated); cost:%.3f(sec);\n"
		,d->counter, (double)(cost / d->counter)
		,(double)(d->counter / cost)
		,cost);	
	printf("COUNT:%ld READCOUNT:%d WRITECOUNT:%d PERCENT:%f \n",d->counter,countread,countwrite,d->percent);	
		
	return 0;				//prosthikh dikh mas gt den termatize alliws giati o compiler den evlepe an termatizei h synarthsh
}

//-------------------------------END OF PROSTHETOS CODE--------------------------------------------------

/*
Dato un file binario contenente un sequenza di 2^15 interi di tipo short, scrivere un programma che crea N processi o threads,
i quali leggono il contenuto del file ed individuano il valore minimo e massimo contenuto nel file.
Nel fornire una soluzione rispettare i seguenti vincoli:
	-ciascun intero non può essere letto da più di un thread/processo;
	-ciascun thread/processo può leggere il medesimo intero al più una volta;
	-ciascun thread/processo può allocare memoria nell’heap per al più 512 byte;
	-N è un parametro definito a tempo di compilazione o tramite linea di comando;
	-N è minore o uguale a 8;
	-è ammesso allocare di variabili globali (data) e locali (stack) per memorizzare tipi primitivi
	(puntatori, int, short, char, long, etc.) per al più 128 byte;
	-per generare il file utilizzare la soluzione dell’esercizio 3.1.
*/

 #include <stdio.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <limits.h>

 #define abort(msg) do{printf("%s\n",msg );exit(1);}while(1)


 int ifd;
 short max_global;
 short min_global;
 #define NUMBERS 32768 //2^15
 #define FILE_SIZE = NUMBERS*(sizeof(short))
 #define MEM_MAX  512
 pthread_spinlock_t th_lock;

void *thread_func(void *val){
	short min= SHRT_MAX;
	short max= SHRT_MIN;

	int size_r;
	int *buffer = malloc(MEM_MAX);
	while(size_r=read(ifd,buffer,MEM_MAX)>0){
		for(int i=0;i<(size_r/sizeof(short));i++){
			if (buffer[i]<min) min = buffer[i];
			if (buffer[i]<max) max = buffer[i];
		}
	}
	pthread_spin_lock(&th_lock);
	if (max_global<max) max_global=max;
	if (min_global>min) min_global=min;
	pthread_spin_unlock(&th_lock);
	pthread_exit(NULL);
}



int main(int argc, char const *argv[])
{
	int n_threads;

	/*controllo i campi inseriti*/
	if(argc!=3)
		abort("errore: inserire numero di threads e nome del file");

	n_threads=atoi(argv[1]);
	if(n_threads>8 || n_threads<1)
		abort("errore: il numero di threads deve essere compreso tra 1 e 8");

	/*apro il file*/
	ifd=open(argv[2],O_RDONLY);
		if(ifd==-1)
			abort("erroe: apertura file");

	/*creo i threads*/
	pthread_t *threads = malloc(n_threads *(sizeof(pthread_t)));

	pthread_spin_init(&th_lock,PTHREAD_PROCESS_PRIVATE);

	for (int i = 0; i < n_threads; ++i)
		pthread_create(&threads[i],NULL,thread_func,NULL);//non ne sono sicuro

	for (int i = 0; i < n_threads; ++i)
		pthread_join(threads[i],NULL);

	close(ifd);
	free(threads);

	printf("max= %d\n", max_global);
	printf("min= %d\n", min_global);

	close(ifd);
}
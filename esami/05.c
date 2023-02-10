/* ********************************************************************
 *TESTO D'ESAME:
 *Si scriva un programma C tale che il main thread legge ciclicamente da standard input una stringa P
 *che corrisponde ad un percorso di file. Per ciascun percorso P viene creato un nuovo thread, il quale
 *legge il file al percorso P cercando il carattere ‘a’.
 *Qualora il main thread riceve un percorso non valido o un numero di percorsi pari a N (a piacere) il
 *programma stampa su standard output il numero di file che contengono il carattere ‘a’.
 *Nota: la lettura da standard input del main thread dovrà essere concorrente alla lettura dei file da
 *parte dei thread child.
 * ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#define N_CICLI 2


char **files;
pthread_t *threads;
int rispetto;
char carattere = 'a';

pthread_spinlock_t th_lock;

void *thread_fun(void *val){
char *file= *((char*) val);
	int ifd=open(file,O_RDONLY);
		if(ifd==-1)
			printf("problemone!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	char buffer;

	printf("sto per entrare nel do\n");

	do{
		read(ifd,buffer,sizeof(char));
		if(strcmp(buffer,carattere)){
			pthread_spin_lock(&th_lock);
			rispetto++;
			pthread_spin_unlock(&th_lock);
			buffer=0;
		}

	}while(buffer>0);
	close(ifd);
}

int main(int argc, char const *argv[])
{
	int usati=0;
	files=malloc(N_CICLI * sizeof(char**));
	threads=malloc(sizeof(pthread_t)*N_CICLI);
	char* prova;

	pthread_spin_init(&th_lock,PTHREAD_PROCESS_PRIVATE);

	for(int i =0;i<N_CICLI;i++){
		printf("inserisci una directori\n");
		scanf("%s\n",files[i]);
		pthread_create(&threads[i],NULL,thread_fun,&files[i]);
	}

	for (int i = 0; i < N_CICLI; ++i)
		pthread_join(threads[i],NULL);

	printf("i file che rispettano il criterio sono: %d\n", rispetto);



	free(files);
	free(threads);
}
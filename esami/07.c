/*
Si scriva un programma C che prende come argomenti una sequenza di nomi di file.
Per ciascuna coppia di file, il programma crea un nuovo thread/processo il quale:
• se i due file possono essere letti ed hanno la stessa taglia
• scrive su standard output il nome dei due file analizzati e l’esito del controllo.
Il programma prima di terminare scrive su standard output il numero di coppie di file che hanno superato il controllo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 

#define abort(msg) do{printf("%s\n",msg);exit(1);} while(0)

char **files;
int superato;
pthread_spinlock_t th_lock;





void *thread_fun(void *val){
	int id=*(int*) val;
	int ifd1,ifd2;
	ifd1=open(files[id*2],O_RDONLY);
	ifd2=open(files[id*2+1],O_RDONLY);
	if(ifd1!=-1 && ifd2!=-1 && (lseek(ifd1,0,SEEK_END) == lseek(ifd2,0,SEEK_END))) {
		printf("il file %s e il file %s sono lunghi uguali\n",files[id*2],files[id*2+1]);
		pthread_spin_lock(&th_lock);
		superato++;	
		pthread_spin_unlock(&th_lock);
	}
	close(ifd1);
	close(ifd2);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	if(argc%2==0 || argc<2)
		abort("error: 07 <fileA1> <fileA2>...");
	int n_thread= (argc-1)/2;

	files= malloc((argc-1)*(sizeof(char**)));
	for (int i = 0; i < argc-1; ++i)
		files[i]=argv[(i+1)];
	int indici[n_thread];
	for(int i =0; i<n_thread;i++)
		indici[i]=i;

	pthread_t *threads=malloc(sizeof(pthread_t)* n_thread);
	pthread_spin_init(&th_lock,PTHREAD_PROCESS_PRIVATE);

	for (int i = 0; i < n_thread; ++i)
		pthread_create(&threads[i],NULL,thread_fun,&indici[i]);

	for (int i = 0; i < n_thread; ++i)
		pthread_join(threads[i],NULL);

	printf("ci sono state %d coppie di file che hanno superato i controlli\n", superato);

	free(files);
	free(threads);
	return 0;
}
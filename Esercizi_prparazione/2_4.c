/*
	
	Scrivere un programma in C che:
		prende inizialmente una stringa da input (può contenere anche spazi bianchi) e la salva in un buffer
		fork-a un processo figlio che manda in stampa la stessa stringa acquisita dal processo padre.
		Il processo padre termina solo dopo che il processo figlio ha terminato (verificare che tale ordine è rispettato stampando i PID dei processi).
*/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

#define LUNG_STRINGA 2

char stringa[LUNG_STRINGA];

void *child_func(void *par){
	*((int*)par)=1;

	printf("Stampo la stringa presa da mio padre: %s, io sono il thread n: %ld\n",stringa, pthread_self());
	pthread_exit(par);


}

int main(int argc, char const *argv[])
{
	pthread_t p;
	int res, *status_ptr, status_val;
	*status_ptr=status_val;		//punto all'indirizzo dello status_val
	printf("sono il padre e devo prendere una stringa come input: ");
	scanf("%s",stringa);
	printf("sono il padre e devo creare il figlio\n");
	res=pthread_create(&p,NULL,child_func,&status_val);
	if(res!=0)
		printf("crazione figlio fallita\n");
	else{
		printf("Sono il padre, ho creato mio figlio %lu e aspetto che finisca\n",p);
		pthread_join(p,(void**) &status_ptr);					//aspetto che mio figlio finisca e retituisco il controllo al padre
		printf("mio figlio ha finito invocando %d\n",*status_ptr);
	
	}

	printf("sono il padre, e' ora di finire\n");
	return 0;
}
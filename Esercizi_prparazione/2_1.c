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

#define LUNG_STRINGA (20)

int main(){
	int res,status;
	char stringa[LUNG_STRINGA];
	printf("Sono il padre, inserisci una parola: ");
	scanf("%s",stringa);
	printf("Sono il padre devo creare mio figlio\n");
	res=fork();
	if(res==-1)
		printf("Ho fallito la creazione di mio figlio\n");
	else 
		if(res==0){
			sleep(3);
			printf("Sono il figlio, mando in stampa la parola presa da mio padre: %s\n",stringa);
			exit(1);
		}
		else{
			printf("Sono il padre, attendo che mio figlio finisca\n");
			wait(&status);
			printf("Sono il padre, mio figlio ha invocato %d\n",WEXITSTATUS(status));
			printf("Sono il padre,e' ora di morire\n");
		}

	exit(0);

}
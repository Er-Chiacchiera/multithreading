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

int main()
{
	/*dichiaro le variabili*/
	int res,status;
	char stringa[LUNG_STRINGA];		//variabile condivisa con il processo figlio(guadre teoria)

	/*processo del padre*/
	printf("Sono il padre, inserisci una parola: ");
	scanf("%s",stringa);
	printf("Sono il padre devo creare mio figlio\n");
	res=fork();			//invoco la fork (creo un processo figlio)
	if(res==-1)			//errore sulla fork
		printf("Ho fallito la creazione di mio figlio\n");
	else 
		if(res==0){		//sono il processo figlio
			sleep(3);
			printf("Sono il figlio, mando in stampa la parola presa da mio padre: %s\n",stringa);
			exit(1);
		}
		else{			//sono il processo padre
			printf("Sono il padre, attendo che mio figlio finisca\n");
			wait(&status);		//attendo fino a che mio figlio non finisca
			printf("Sono il padre, mio figlio ha invocato %d\n",WEXITSTATUS(status));
			printf("Sono il padre, e' ora di morire\n");
		}

	exit(0);

}
/*
	Scrivere un programma che:
		prende una stringa da tastiera e la inserisce all’interno di un buffer allocato dinamicamente nella heap da parte della funzione scanf().
		Copia poi tale stringa all’interno di un secondo buffer allocato sullo stack della taglia necessaria a contenerla.
		Libera quindi il buffer allocato nella heap.
		Stampa sullo schermo la stringa copiata nel buffer allocato sullo stack.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LUNG_HEAP (20)

int main(int argc, char const *argv[])
{
	
	char *heap;
	int lung;
  	heap=(char*) malloc(sizeof(char) * LUNG_HEAP );			//alloco la memoria sull'heap
  	printf("inserisci la stringa da allocare neel'heap: ");
  	scanf("%s",heap);										//leggo il valore da tastiera
  	printf("hai inserito la string sul heap: %s\n",heap);
  	
  	lung = strlen(heap);									//calcolo il la lunghezza della stringa
  	printf("lunghezza array statico %d\n",lung);
  	char array[lung];
  	for (int i = 0; i < lung; ++i)							//trasferisco la stringa sullo stack
  	{
  		array[i]=heap[i];
  	}
  	printf("ho messo la stringa sullo stack: %s\n",heap);
  	free(heap);												//libero la memoria sull'heap
  	printf("ho lioberato la memoria dall'heap\n");



}
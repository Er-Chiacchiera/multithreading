/*
	Scrivere un programma che:
		prende una stringa passata come primo argomento (i.e. char *argv[]) al programma stesso quando questo viene eseguito.
		Copia tale stringa all’interno di un buffer di dimensione fissa facendo attenzione a non superare il limite imposto dalla taglia, e stamparla quindi sullo schermo.
		Rigira la stringa (primo carattere in ultima posizione, secondo carattere in penultima posizione, ecc.) senza fare utilizzo di un ulteriore buffer
		Stampa anche questa stringa sullo schermo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNG_HEAP (20)

int main(int argc, char const *argv[])
{
	char *heap;
	int lung;
	heap=(char*) malloc(sizeof(char) * LUNG_HEAP);
	lung = 	strlen(argv[1]);
	for (int i = 0; i < lung; i++)
	{
		heap[i]=argv[1][i];
	}

	printf("nell'heap ho messo la stringa: %s, ora procedo per invertirla\n",heap );

	for (int i = 0; i < (lung/2); i++)
	{
		int a;
		a=heap[i];
		heap[i]=heap[(lung-(i+1))];
		heap[(lung-(i+1))]=a;
	}

	printf("la stringa invertita e': %s\n",heap);
}
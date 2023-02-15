/*
    Scrivere un programma in C che:

        prende inizialmente una stringa da input (può contenere anche spazi bianchi) e la salva in un buffer
        fork-are 2 processi figli che contribuiscono a stampare la stringa inversa della stringa acquisita dal processo padre.
        Il processo padre termina solo dopo che i processi figli hanno terminato.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define DIM_STRINGA_MAX 100
#define N_THREAD 2

char stringa1[DIM_STRINGA_MAX];
char stringa2[DIM_STRINGA_MAX];





void *thread_Func(void * par){
     
    int id =*((int*)par);           // mi prendo l'id dal parametro di funzione (cast a int)
    int lSezGeneral=strlen(stringa1)/N_THREAD;      //calcolo la lunghezza generale senza tenr conto dell'ultima stringa
    int resto=0;

    sleep(id);                      //solo per pulizia
    if(id==(N_THREAD-1))            //sono l'ultimo trhead;
        resto=((strlen(stringa1))%N_THREAD);


    printf("sono il thread <%d>, inverto la mia sezione che e' lunga %d \n",id,(lSezGeneral+resto));

    char stringaProv[(lSezGeneral+resto)];      //creo la stringa provvisoria
    int j=((lSezGeneral-1)+resto);              //setto il contatore a dove devo iniziare ad invertire la stringa


        /*inverto la stringa nella stringa provvisoria*/
        for (int i = (id*lSezGeneral); i < ((id*lSezGeneral)+lSezGeneral+resto); i++)
        {
            stringaProv[j]=stringa1[i];
            j--;
        }

        /*copio la stringa provvisoria nella stringa finale*/

        if(id==(N_THREAD-1)){                               //sono l'ultimo thread
            for(int i=0; i<(lSezGeneral+resto); i++)
                stringa2[i]=stringaProv[i];
        }

        else{                                               //sono il thread da 0 a n-1
            resto=((strlen(stringa1))%N_THREAD);
            j=((((N_THREAD-1)-id)*lSezGeneral)+resto);      //setto il punto dove devo iniziare a scrivere
            for(int i =0;i<lSezGeneral;i++){
                stringa2[j]=stringaProv[i];
                j++;
            }
        }

        printf("sono il thread <%d>, ho fatto il mio dovere\n",id);
    pthread_exit(NULL);
}


/*controllo se tutti i trhead sono stati creati*/
int hasCreate(int *res){
    int boolean = 1;

    for (int i = 0; i < N_THREAD; i++)
    {
        if(res[i]!=0)
            boolean=0;
    }

    return boolean;
}

int main(int argc, char const *argv[]){
    
    /*creo gli id per i thread*/
    int id[N_THREAD];
    for (int i = 0; i < N_THREAD; i++)
        id[i]=i;

    /*prendo la stringa in input*/
    printf("inserisci la parola che vuoi: ");
    scanf("%s",stringa1);

    printf("sono il padre vado a creare i threads\n");

    /*alloco lo spazio per i trhead e per le risposte di creazione*/
    pthread_t *tids= malloc(N_THREAD *sizeof(pthread_t));
    int *res =  malloc(N_THREAD *sizeof(int));

    /*creo i trhead*/
    for (int i = 0; i < N_THREAD; i++)
    {
        res[i]=pthread_create(&(tids[i]),NULL,thread_Func,&id[i]);
    }
    /*Se tutti i trhead sono stati creati allora mando l'errore in stampa */
    if(!(hasCreate(res)))
        printf("ERRORE trhead non creati\n");
    


    else{
        /* Aspetto che tutti i figli finiscono*/
        printf("Ho creato tutti i miei figli\n");
        
        for (int i = 0; i < N_THREAD; ++i)
            pthread_join(tids[i],NULL);

        printf("i miei figli hanno fatto, stampo la stringa: \n");
        printf("%s\n",stringa2);
        
    }
    return 0;
}
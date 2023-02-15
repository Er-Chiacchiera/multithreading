/*
    Una applicazione ha p thread produttori e c thread consumatori.
    L’applicazione lavora in due fasi. Nella prima fase i thread produttori Pi inseriscono in una coda vuota c elementi.
    Nella seconda fase i thread consumatori Ci estraggono ciascuno un elemento dalla coda a partire da C1 a Cc.
    Le due fasi si alternano per un tempo infinito.
    Scrivere in pseudocodice il corpo delle funzioni eseguite da un generico thread Pi e Ci assumendo che:

        la coda è inizialmente vuota;

        l’implementazione disponibile della coda NON è thread safe;

        è ammesso solo l’utilizzo di semafori;

        non è ammesso utilizzare altre variabili condivise ad eccezione di eventuali semafori e della coda.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>


#define N_THREAD 4

#define abort(msg); do{printf("%s\n",msg);exit(0)}while(1);

char stringa[N_THREAD];
pthread_spinlock_t th_lock;     //creo un lock
int turn;

void *thread_Prod_Func(void *par){
    int id =*((int*)par);
    while(turn!=id);        //aspetto che sia il mio turno
    pthread_spin_lock(&th_lock);
    stringa[id]=id;
    (turn++)%4;
    sleep(2);
    pthread_spin_unlock(&th_lock);
    printf("ho caricato la stringa %d\n", id);
    pthread_exit(NULL);
     
}

void *thread_Cons_Func(void *par){
    int val;
    int id =*((int*)par);
    pthread_spin_lock(&th_lock);
    val=stringa[id];
    stringa[id]=0;
    printf("ho levato la stringa %d\n", val);
    pthread_spin_unlock(&th_lock);
    pthread_exit(0);
}



int main(int argc, char const *argv[]){

    /*Credo gli id da passare come parametro*/
    int id[N_THREAD];
    for(int i=0;i<N_THREAD;i++)
        id[i]=i;
    

    /*alloco lo spazio per i trhead e per le risposte di creazione*/
    pthread_t *trhead_prod = malloc(N_THREAD*(sizeof(pthread_t)));
    pthread_t *trhead_cons = malloc(N_THREAD*(sizeof(pthread_t)));

    int res;

    /*inizializzo il lock*/
    pthread_spin_init(&th_lock,PTHREAD_PROCESS_PRIVATE);

    /*iniziaizzo l'array di thread produttori*/
    for(int i=0;i<N_THREAD;i++){
        res= pthread_create(&trhead_prod[i], NULL, thread_Prod_Func, &id[i]);
        if(res!=0)
            abort("error: thread produttore non creato");

    }

    /*ricongiungo tutti i miei figli*/
    for(int i=0;i<N_THREAD;i++)
        pthread_join(trhead_prod[i],NULL);


    /*iniziaizzo l'array di thread produttori*/
    for(int i=0;i<N_THREAD;i++){
        res= pthread_create(&trhead_cons[i],NULL,thread_Cons_Func,&id[i]);
        if(res!=0)
            abort("error: thread consumatore non creato");
    }

    for(int i=0;i<N_THREAD;i++)
        pthread_join(trhead_cons[i],NULL);


    free(trhead_cons);
    free(trhead_prod);

}
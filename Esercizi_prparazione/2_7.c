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
#include <semaphore.h>
#include <string.h>


#define N_THREAD 4

char q[N_THREAD];
psem_t sem_cons;
psem_t sem_prod;
int turn;

void *thread_Prod_Func(void *par){
    int id =*((int*)par);
    while(turn!=id);
    wait(&sem_prod);
    if(q.zize()<max_size)
        push(stringa,id);
        sem_post(sem_prod)
    else
        sem_post(sem_cons);
    sleep(2);
    pthread_exit(NULL);
     
}

void *thread_Cons_Func(void *par){
    int val;
    int id =*((int*)par);
    wait(sem_cons);
    if(q.size()!=0 && id=max_size-q.size())
        pop(q);
    else{
        if(q.size()!=0)
            sem_post(sem_cons);
            thread_Cons_Func(&id);
        else
            pthread_exit(NULL);
    pthread_exit(NULL);

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

    /*inizializzo i semafori*/
    sem_init(sem_cons,0,1);
    sem_init(sem_prod,0,0);

    /*iniziaizzo l'array di thread produttori e utilizzatori*/
    for(int i=0;i<N_THREAD;i++){
        res= pthread_create(&trhead_prod[i], NULL, thread_Prod_Func, &id[i]);
        if(res!=0){
            printf("thread non creato\n");
            exit(0);
        }

    }

    /*ricongiungo tutti i miei figli*/
    for(int i=0;i<N_THREAD;i++)
        pthread_join(trhead_prod[i],NULL);


    /*iniziaizzo l'array di thread produttori*/
    for(int i=0;i<N_THREAD;i++){
        res= pthread_create(&trhead_cons[i],NULL,thread_Cons_Func,&id[i]);
        if(res!=0){
            printf("thread non crato\n");
            exit(0);
        }
    }

    for(int i=0;i<N_THREAD;i++)
        pthread_join(trhead_cons[i],NULL);

}
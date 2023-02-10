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
#include <string.h>

#define LUNG_STRINGA (20)

#define N_PROCESS (2)



void stringInvert(char * stringa){
    char prov;               //provvisoria
    int lung = (strlen(stringa));
    for (int i = 0; i < (lung/2); i++)
    {
        prov = stringa[i];
        stringa[i]=stringa[(lung-(i+1))];
        stringa[(lung-(i+1))]=prov;
    }

}

int main(int argc, char const *argv[])
{
    int res, status;
    int cont=0;
    char stringa[LUNG_STRINGA];

    pid_t pidP =getpid();               //prendo il pid del padre
    printf("Sono il padre, inserisci una parola: ");
    scanf("%s",stringa);
    stringInvert(stringa);
    printf("Sono il padre <%d> devo creare mio figlio: \n",getpid());


    /*Creo il numero di figli che ho impostato ?*/
    for (int i = 0; i < N_PROCESS && getpid()==pidP; i++)
    {
        res=fork();
    }

    printf("Sono il processo: <%d>\n",getpid());
    sleep(2);

    /*entro nel ciclo a seconda se sono il padre o il figlio*/
    if(res==0){         //sono il figlio
        sleep(3);
        printf("sono il figlio <%d> nel true, mio padre ha salvato la parola: %s\n",getpid(),stringa);
        exit(getpid());
    }
    else{               //sono il padre
        printf("sono il padre <%d> nell'else\n",getpid());

        while(cont<2){
            wait(&status);

            if(WEXITSTATUS(status)!=7){
                printf( "e' stato invocato l'exit : %d\n", WEXITSTATUS(status));
                cont++;
                status=0;
            }
        }
        printf("Sono il padre che crepa\n");
        exit(1);
    }

    exit(0);
}
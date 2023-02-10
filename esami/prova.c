/*********************************************************************
 *  _____  ______          _____  __  __ ______
 * |  __ \|  ____|   /\   |  __ \|  \/  |  ____|
 * | |__) | |__     /  \  | |  | | \  / | |__
 * |  _  /|  __|   / /\ \ | |  | | |\/| |  __|
 * | | \ \| |____ / ____ \| |__| | |  | | |____
 * |_|  \_\______/_/    \_\_____/|_|  |_|______|
 *
 * *******************************************************************
 *
 *
 * DO NOT FORGET TO FILL THE FOLLOWING WITH YOUR PERSONAL DATA
 * First Name:
 * Last Name:
 * Student Id:
 *
 * ********************************************************************
 *TESTO D'ESAME:
 *Si scriva un programma C che prende come argomenti una sequenza di nomi di file.
 *Per ciascuna coppia di file, il programma crea un nuovo thread/processo il quale:
 *-se i due file possono essere letti ed hanno la stessa taglia
 *scrive su standard output il nome dei due file analizzati e l’esito del controllo.
 *Il programma prima di terminare scrive su standard output il numero di coppie di file che hanno
 *superato il controllo.
 *Nota: il controllo di coppie distinte deve essere concorrente.
 * ***********************************/


/*****************************
 * READY TO USE HEADERS
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int *checked;
char **filenames;


#define abort(msg) do{printf(msg); exit(1);} while(0)

/***********************************
 * Complete the following function
 ***********************************/
void* checker(void* param){
  int id = (int*) param;

  //file descriptors for files
  int file1 = open(filenames[(id*2)], O_RDONLY);
  int file2 = open(filenames[(id*2)+1], O_RDONLY);

  if((file1!=-1 && file2!=-1) && (lseek(file1, 0, SEEK_END)==lseek(file2, 0, SEEK_END))){
    checked[id]++;
    printf("\nThe couple of files %s, %s passed all the checks\n", filenames[id*2], filenames[id*2+1]);
  } else {
    printf("\nThe couple of files %s, %s didn't passed all the checks\n", filenames[id*2], filenames[id*2+1]);

  }

  close(file1);
  close(file2);
  return 0;
}

int main(int argc, char *argv[]) {
  if(argc%2==0)
    abort("Too few arguments, usage: ./esame_07 <file1> <file2> ...");


  //a thread for any couple of files
  int n_threads = (argc-1)/2;

  printf("\nNumber of threads used:%d\n", n_threads);

  pthread_t tids[n_threads];

  filenames = (char**) malloc(sizeof(char**)*(argc-1));
  checked = (int*) malloc(sizeof(int)*n_threads);
  for(int i=1; i<argc; i++){
    filenames[i-1] = argv[i];
  }

  printf("\nFILENAMES:\n");
  for(int i=0; i<argc-1; i++){
    printf("%s\n",filenames[i]);
  }

  printf("\nSTARTING MULTITHREAD FILE CHECKER\n");
  printf("__________________________________\n");

  for(int i=0; i<n_threads; i++){
    pthread_create(tids+i, NULL, checker, (void*) i);
  }

  int total = 0;
  for(int i=0; i<n_threads; i++){
    pthread_join(tids[i], NULL);
    total += checked[i];
  }
  printf("__________________________________\n");
  printf("\nEND OF MULTITHREAD FILE CHECKER\n");

  printf("\n%d couple/couples of files passed all the checks!\n", total);

  free(filenames);
  free(checked);
}

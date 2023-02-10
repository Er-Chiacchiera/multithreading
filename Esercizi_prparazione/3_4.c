/*Si scriva il codice di una funzione C con la seguente interfaccia void tunnel(int descriptors[], int count) tale che,
se eseguita, porti l’applicazione a gestire,
per ogni file-descriptor dell’array descriptors l’inoltro del flusso dei dati in ingresso verso il canale di standard-output dell’applicazione.
Il parametro count indica di quanti elementi è costituito l’array descriptors. L’inoltro dovrà essere attuato in modo concorrente per i diversi canali.

*/

 #include <stdio.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>

#define STDOUT 1

 #define abort(msg) do{printf("%s\n",msg);exit(1);} while(0)



 void *thread_func(void *val){
 	int size_w,size_r;
 	int descri =*((int*) val);
 	int buffer_size = lseek(descri,0,SEEK_END);
 	char buffer_prov[buffer_size];
 	lseek(descri,0,SEEK_SET);
 	
 	size_r=read(descri,buffer_prov,buffer_size);
 	if(size_r==-1)
 		abort("error: lettura file");

 	size_w=write(STDOUT,buffer_prov,size_r);
 		if(size_r!=size_w)
 			abort("error:scrittura file");

 	pthread_exit;

 }

 void tunnel(int descriptors[], int count){

 	pthread_t threads[count];
 	for (int i = 0; i < count; ++i)
 		pthread_create(&threads[i],NULL,thread_func,&descriptors[i]);

 	for (int i = 0; i < count; ++i)
 		pthread_join(threads[i],NULL);

 }

int main(int argc, char const *argv[])
{
	if(argc<2)
		abort("error: 3_4 <file1.txt>");
	int count=argc-1;
	int descriptors[count];
	/*faccio la open per tutti gli argomenti di argv*/
	for (int i = 0; i < count; ++i)
		descriptors[i]=open(argv[i+1],O_RDONLY);

	tunnel(descriptors,count);

	for (int i = 0; i < count; ++i)
		close(descriptors[i]);

}
/*Scrivere un programma C invert che dato un file A ne inverte il contenuto e lo memorizza in nuovo file B. Il programma deve:

	-riportare il contenuto di A in memoria;
	-invertire la posizione di ciascun byte utilizzando un numero N di thread/processi concorrenti;
	-scrivere il risultato in un nuovo file B;
	-prendere A, B e N come argomenti da linea di comando.

*/

 #include <stdio.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>

 #define abort(msg) do{printf("%s\n",msg);exit(1);} while(0)
 off_t buffer_size;
 char *buffer_input;
 char *buffer_output;
 int n_threads;


 void *thread_func(void *val){
 	int index = *((int*)val);		//che n thread sono
 	int last =(index==n_threads-1);	//sono l'ultimo thread ?
 	/*calcolo la lunghezza della sezione*/
 	int l_sez=buffer_size/n_threads;
 	int rest = buffer_size%n_threads;
 	if(last)
 		l_sez=l_sez+rest;
 	printf("sono il trhed n %d e la mia sezione e' lunga %d\n",index,l_sez);

 	/*inverto la mia sezione delle stringa in un buffer provvisorio*/

	//printf("ho letto la stringa: %s\n",buffer_input);
 	char buffer_prov[l_sez];

 	if(last)
 		for(int i = ((l_sez-rest)*index); i < buffer_size; ++i)
 			buffer_prov[(buffer_size-1)-i]=buffer_input[i];
 	else
 		for (int i = (l_sez*index); i < l_sez*(index+1); ++i)
 			buffer_prov[(l_sez*(index+1))-1-i]=buffer_input[i];

 	printf("sono il trhed n %d e la mia sezione e' lunga %d e la mia stringa provvisoria e': %s \n\n",index,l_sez,buffer_prov);
 	/*inserisco la stringa del buffer provvisorio nella stringa finale*/
 	if(last){
 		for (int i = 0; i < l_sez; ++i)
 			buffer_output[i]=buffer_prov[i];
 	}
	else{
		int j=0;
 		for(int i =((n_threads-1-index)*l_sez)+rest;i<(((n_threads-index)*l_sez)+rest);i++){
 			buffer_output[i]=buffer_prov[j];
 			j++;
 		}
 	}

 	pthread_exit(NULL);
 }

int main(int argc, char const *argv[])
{
	/*dichiarazioni variabli*/
	int ifd,ofd;
	int size_r,size_w;

	/*controllo che siano stati inseriti tutti i campi durante l'esecuzione*/
	if(argc!=4)
		abort("error: 3.3 <nameA.txt> <nameB.txt> <numero di threads>");

	/*prendo il numero di threads*/
	n_threads = atoi(argv[3]);
	if(n_threads<1)
		abort("error: n_threads<0");

	/*apro il file e controllo che non ci siano problemi*/
	ifd=open(argv[1],O_RDONLY);
	if (ifd==-1)
		abort("errore: file di apertura da leggere");

	/*apro il file per scrivere e controllo che non ci siano problemi*/
	ofd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0660);
	if(ofd==-1)
		abort("errore: file di output apertura");

	/*Alloco lo sapzio per la memoria dei buffer*/
	buffer_size=lseek(ifd,0,SEEK_END);	//quanto è lunga la mia stringa
	lseek(ifd,0,SEEK_SET);				//imposto il "puntatore all'inizio"
	buffer_input= malloc(buffer_size);	
	buffer_output= malloc(buffer_size);
	printf("il buffer e' lungo %ld\n",buffer_size);

	/*copio il numero di bit che voglio*/
	size_r=read(ifd,buffer_input,buffer_size);
	if(size_r==-1)
		abort("errore: read file");
	printf("ho letto:\nn_byte %d\ne la stringa: %s\n",size_r,buffer_input);

	/*****************************gestione dei threads*****************************/

	/*creazione dell'indice*/
	int threads_index[n_threads];

	/*allocazione spazio per i threads*/
	pthread_t threads[n_threads];

	/*creazione dei threads*/
	for (int i = 0; i < n_threads; ++i){
		threads_index[i]=i;
		pthread_create(&threads[i],NULL,thread_func,&threads_index[i]);
	}

	/*aspetto che ritornino i threads*/
	for (int i = 0; i < n_threads; ++i)
		pthread_join(threads[i],NULL);

	/**************************termine gestione dei threads*************************/
	printf("la stringa da incollare e': %s\n",buffer_output);

	/*incollo il numero di bit che ho copiato*/
	size_w=write(ofd,buffer_output,size_r);
	if(size_w==-1)
		abort("errore: write file");

	/*chiudo tutto*/
	close(ifd);
	close(ofd);
	free(buffer_input);
	free(buffer_output);
}
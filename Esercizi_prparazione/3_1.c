/*Nei sistemi operativi UNIX, /dev/urandom è un dispositivo a caratteri (char device) virtuale in grado di generare numeri casuali.
Nello specifico, l’operazione di lettura dal relativo file produce byte casuali.
Scrivere un programma C che genera un file con contenuto interamente randomico. Il programma:

    -prende come parametri da linea di comando: un numero N e una stringa S da usare come nome del file da creare;
	-crea un file S contenente N byte randomici;
    -utilizza il dispositivo /dev/random come sorgente di numeri pseudo-casuali.

*/

 #include <stdio.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <unistd.h>

 #define abort(msg) do{printf("%s\n",msg);exit(1);} while(0)

int main(int argc, char const *argv[])
{
	/*dichiarazioni variabli*/
	int ifd,ofd;
	int size_r,size_w;

	/*controllo che siano stati inseriti tutti i campi durante l'esecuzione*/
	if(argc!=3)
		abort("error: nome <numero bit, nome nuovo file");

	/*definisco la dimensione del buffer e lo alloco*/
	int buffer_size = atoi(argv[1]);
	char buffer[buffer_size];

	/*apro il file e controllo che non ci siano problemi*/
	ifd=open("/dev/urandom",O_RDONLY);
	if (ifd==-1)
		abort("errore: file di apertura da leggere");

	/*apro il file per scrivere e controllo che non ci siano problemi*/
	ofd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0660);
	if(ofd==-1)
		abort("errore: file di output apertura");

	/*copio il numero di bit che voglio*/
	size_r=read(ifd,buffer,buffer_size);
	if(size_r==-1)
		abort("errore: read file");

	/*incollo il numero di bit che ho copiato*/
	size_w=write(ofd,buffer,size_r);
	if(size_w==-1)
		abort("errore: wreite file");

	/*chiudo tutto*/
	close(ifd);
	close(ofd);
}
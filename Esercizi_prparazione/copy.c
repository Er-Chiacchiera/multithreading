 #include <stdio.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <unistd.h>

#define BUFSIZE 250
#define abort(msg) do{printf("%s\n",msg);exit(1);}while(0)

 int main(int argc, char const *argv[])
{
	/*dichiariazione delle variabili*/
	int size_r, size_w;
	int ifd, ofd;
	int end=0;
	char buffer[BUFSIZE];

	/*controllo che ci siano i parametri inseriti*/
	if (argc!=3)
		abort("inserire i file");

	/*apro il file da copiare*/
	ifd=open(argv[1],O_RDONLY);
	if(ifd==-1)
		abort("file input non valido\n");

	/*apro file da mettere*/
	ofd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0660);
		if(ofd==-1)
			abort("file output non valido");

	while(!end)
	{
		/*copio il file*/
		size_r=read(ifd,buffer,BUFSIZE);
			if(size_r==-1)
				abort("errore in lettura file");
		end= (size_r==0);

		/*incollo il file nell'output*/
		size_w=write(ofd,buffer,size_r);
		if(size_w==-1)
			abort("errore in scrittura file");
		printf("ho scitrro %d Byte\n",size_w);
	}

	close(ifd);
	close(ofd);


	return 0;
}
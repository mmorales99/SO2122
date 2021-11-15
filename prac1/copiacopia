// Manuel Morales Amat 48789509T

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	if(argc != 3) exit(EXIT_FAILURE);
	int src = open(argv[1], O_RDONLY);
	if(src == -1) exit(EXIT_FAILURE);
	else{
		// abrir tuberia
		int tub[2];
		pipe(tub);
		char buff[2];
		if(fork()){ // proceso padre
			while (	read(src,buff,sizeof(char)) != 0)
				write(tub[1],buff,sizeof(char));
			close(src);
		}else{ //hi| procesojo
			int dst = open(argv[2],O_WRONLY|O_CREAT, 0666);
			if(dst==-1) exit(EXIT_FAILURE);
			while (	read(tub[0],buff,sizeof(char)) != 0) {
				write(dst,buff,sizeof(char));
			}
			close(dst);
		}
		close(tub[0]);
		close(tub[1]);
	}
	exit(EXIT_SUCCESS);
}
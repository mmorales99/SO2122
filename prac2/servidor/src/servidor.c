#include "lib.h"

bool isGoogle(){
	FILE* fp = fopen("Google.html","r");
	if(!fp) return 0;
	else {
		fclose(fp);
		return 1;
	}
	return -1;
}

void getGoogle(){
	system("wget google.com && mv index.html Google.html");
}

void startServidor(){
	int ss, sc;
	struct sockaddr_in as, ac;
	as.sin_family = AF_INET;
	as.sin_port = htons(9999);
	as.sin_addr.s_addr = INADDR_ANY;
	ss = socket(AF_INET,SOCK_STREAM, 0);
	if(ss<0) perror("NO SE HA PODIDO CREAR EL SOCKET\n\0");
	bind(ss, (struct sockaddr *) &as, sizeof(as));
	int isListening = listen(ss, 5);
	if(isListening<0) perror("ERROR ESCUCHANDO\n\0");
	do{
		int ac_leng = sizeof(ac);
		sc = accept(ss, (struct sockaddr*)&ac, &ac_leng);
		if(sc==-1) continue;
		FILE *fp = fopen("Google.html","r");
		fseek(fp, 0L, SEEK_END);
		long fsize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		char *string = (char *)malloc(fsize + 1);
		fread(string, fsize, 1, fp);
		fclose(fp);
		string[fsize] = 0;
		send(sc,string,fsize+1,0);
		close(sc);
	}while(TRUE);
	close(ss);
}

int main(int argc, char **argv){
	if(!isGoogle()){
		getGoogle();
	}
	startServidor();
	exit(EXIT_SUCCESS);
}
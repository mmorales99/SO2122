#include "lib.h"
#define BUFF_SIZE 99999

void writeBuffToFile(char *buff){
	FILE* fp = fopen("inn.html","w+");
	fwrite(buff,1,sizeof(buff),fp);
	fclose(fp);
}

int main(int argc, char **argv){
	int sc_servidor; 
    sc_servidor = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servidor_info;
    servidor_info.sin_family=AF_INET;
    servidor_info.sin_port=htons(9999);
	if(argc == 2) servidor_info.sin_addr.s_addr = inet_addr(argv[1]);
    else servidor_info.sin_addr.s_addr = INADDR_ANY;

    connect(sc_servidor,(struct sockaddr *)&servidor_info,sizeof(servidor_info));
    char buff[BUFF_SIZE];
    memset(buff,0,sizeof(buff));
    read(sc_servidor,buff,sizeof(buff)); // recv(sc_servidor,buff,sizeof(buff), 0);
    close(sc_servidor);
    printf("%s",buff); fflush(stdin); // writeBuffToFile(buff); system("firefox inn.html");
    return 0;
}
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define LBUF 128
#define LEN sizeof(struct sockaddr_in)
char buffer[LBUF];
int main(int argc, char *argv[]){
	if (argc!=2){
		printf("Client: Need a file name to work.\n");
		exit(1);
	}
	FILE *fil;
	if((fil=fopen(argv[1],"r")) == NULL){
		printf("Client: Cannot open file.\n");
		exit(1);
	}
	fread(buffer, sizeof(buffer),1,fil);
	if(strlen(buffer)>40){
		printf("Client: Datagram bigger than 40 bytes.\n");
		exit(1);
	}
	struct sockaddr_in addr;
	int n, sock;
	if((sock = socket(AF_INET, SOCK_DGRAM,0))<0){
		printf("Client: Unable to create socket.\n");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1305);
	addr.sin_addr.s_addr= htonl(INADDR_ANY);
	if (sendto(sock,buffer,LBUF,0,(struct sockaddr*)&addr, sizeof(addr)) != LBUF){
		printf("Client: Unable to send.\n");
		exit(1);
	}
	for(;;){
		n=recvfrom(sock,buffer,LBUF,0,NULL,NULL);
		if(n<0){
			printf("Client: Recieving error.\n");continue;
		}
		buffer[n]='\0';
		printf("Otvet servera: %s\n", buffer);
		break;
	}
	fclose(fil);
	close(sock);
	exit(0);
}

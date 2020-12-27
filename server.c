#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#define LBUF 128
#define LEN sizeof(struct sockaddr_in)
int main(){
	struct sockaddr_in addr, client;
	char buffer[LBUF];
	int n, sock, s_len;
	int c_len = LEN;
	char buffstr[50];
	time_t rawtime;
	struct tm *timeinfo;
	char timebuffer[10];
	FILE *fil;
	if ((sock = socket(AF_INET, SOCK_DGRAM , 0)) < 0){
		printf("Server: Unable to create socket.\n");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1305);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_len = sizeof(addr);
	if (bind (sock, (struct sockaddr*) &addr, sizeof(addr)) <0){
		printf("Server:Unable to bind socket.\n");
		exit(1);
	}
	for(;;){
		n=recvfrom(sock, buffer, LBUF, 0, (struct sockaddr*) &client, &c_len);
		if (n<0)
		{
			printf("Server:Recieving error.\n");
			continue;
		}
		time (&rawtime);
		timeinfo = localtime (&rawtime);
		strftime(timebuffer, 80, "%H:%M:%S: " , timeinfo);
		buffer[n]='\0';
		strcat(strcpy(buffstr, timebuffer), buffer);
		printf("\n");
		printf("%s",buffstr);
		buffstr[n]='\n';
		if ((fil = fopen("data","a")) == NULL){
			printf("Server:Cannot create file data");
			exit(1);
		}
		fputs(buffstr, fil);
		fclose(fil);
		char msg[] = "Potlog 589-3. you use my server";
		sendto(sock, msg, LBUF , 0, (struct sockaddr *) &client, c_len);
	}
}

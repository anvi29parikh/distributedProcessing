#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include<fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
static int clients = 0;
int main(int argc, char *argv[]){
	int sd, csd, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 2){
	printf("Call model: %s <Port #>\n", argv[0]);
	exit(0);
	}

	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
	fprintf(stderr, "Cannot create socket\n");
	exit(1);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));

	listen(sd, 5);

	while(1){
	csd=accept(sd,(struct sockaddr*)NULL,NULL);
	printf("New connection with client %d\n", clients);
	if(fork()==0)
	child(csd, clients);
	
	}
	close(csd);
}

void child(int sd, int clients){
	char fileName[32];
	snprintf(fileName, sizeof(char) * 32, "file%i.txt", clients);

	int fd = open(fileName, O_CREAT|O_RDWR, 0777);
	char fileContent[255];
	fprintf(stderr,"Now reading content\n");
	if(read(sd, fileContent, 255)>0){
		fprintf(stderr, "client's message: %s\n",fileContent);
		int n=write(fd,fileContent,255);
		fprintf(stderr,"Read bytes %d\n", n);
	}else{
		fprintf(stderr, "not able to read\n");
	}
	close(fd);
	close(sd);
	fprintf(stderr,"Client is dead, wait for a new client\n");
		exit(0);
}













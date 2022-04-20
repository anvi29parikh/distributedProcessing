#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>

int main(int argc, char *argv[]){
	char message[255];
	int ssd, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 5){
	printf("Call model:%s <IP> <Port#> <File1> <File2>\n",argv[0]);
	exit(0);
	}
	
	if((ssd = socket(AF_INET, SOCK_STREAM, 0))<0){
	fprintf(stderr, "Cannot create socket\n");
	exit(1);
	}

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);//convert host bytestream into network bytestream

	if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
	fprintf(stderr, " inet_pton() has failed\n");
	exit(2);
	}


	if(connect(ssd,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
	fprintf(stderr, "connect() has failed, exiting\n");
	exit(3);
	}



	//while(1){
		int fd = open(argv[4], O_RDONLY);
		fprintf(stderr, "Enter the $ sign to quit or a message for the server\n");
		char fileContent[255];
		int n = 255;
		int len_f = lseek(fd,0,SEEK_END);
		lseek(fd,0,SEEK_SET);
		while(n!=0){
			
			printf("len:%d\n",len_f);
			n=read(fd, fileContent,255);
			if((len_f+1)<255){
				n = 0;
			}
			fprintf(stderr, "file's message: %s:::%d\n",fileContent,n);
			write(ssd, fileContent, strlen(fileContent)+1);
		}
		close(fd);
		fprintf(stderr, "written");
	//}
}












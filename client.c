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

	if(argc != 6){
		printf("Call model:%s <IP> <Port#> <Pattern> <File1> <File2>\n",argv[0]);
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
	
	
	int pid=fork();
    
	if(pid==0)
	{
	    printf("Local client output: \n");
	    char *command_args[] = { "grep", "--text", "--with-filename", "-w", argv[3], "--color=always", argv[4], NULL };
	    execvp(command_args[0], command_args);
	    exit(0);
	}
	else if(pid>0)
	{
		waitpid(pid, NULL, 0);
	  int fd = open(argv[5], O_RDONLY);
		write(ssd, argv[3], strlen(argv[3])+1);

		while(1){
			unsigned char fileContent[256] = {0};
			int n = read(fd, fileContent, 256);
			if (n > 0){
				write(ssd, fileContent, n);
			}
			// printf("No of bytes sent in server %i\n", n);
			// printf("sending file content : \n %s", fileContent);

			if (n < 256){	
				break; 
			}
		}
		close(fd);

		fprintf(stderr, "written to server, waiting for results : \n");

		if(read(ssd, message, 255)<0){
			fprintf(stderr, "read() error\n");
			exit(3);
		}
		fprintf(stderr, "server's Output: \n%s ",message);
		
	}
	
	
}












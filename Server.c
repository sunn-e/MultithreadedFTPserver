#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define MAX 50
#define MAX_buff 1025
#define MAX_port 5000
struct sockaddr_in c_addr;
char fname[MAX];//

void* SendFileToClient(int *arg)
{	
	int connectionfd=(int)*arg;
	printf("connection is accepted and id is %d\n",connectionfd);
	printf("connected to client %s:%d\m", inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
	write (connectionfd , fname ,256);
	
	FILE *fp = fopen(fname,"rb");
	if(fp=NULL)
	{
	printf(("File open error");
	return 1;
	}
	while(1)
	{
		unsigned char buff[1024]={0};//read file inchunks of 256bytes but e need 1kb so......meh
		int nread = fread(buff,1,1024,fp);
		printf("bytes read? %d\t",nread);
		if (nread>0)
		{
			printf("Sending\t");
			write(connectionfd, buff,nread);
		}
		if(nread < 1024)
		{
			if (feof(fp))
			{
				printf("end of file\t");
				printf("transfer completed. ID = %d\t",connectionfd);
			}
			if (ferror(fp))
			{
				printf("Error reading\n");
				break;
			}
		}
	}
close(connectionfd);
shutdown(connectionfd,SHUT_WR);//writing
printf("%d's connection closed.\n",connectionfd);
sleep(2);
}
	
int main(int argc , char *argv[])
{
	int connectionfd=0,err;
	pthread_t tid;
	struct sockaddr_in serv_addr;
	int listenfd = 0,ret;
	char sendBuff[MAX_buff];
	int numrv;
	size_t clen=0;//initially zero 
	
	listenfd = socket(AF_INET ,SOCK_STREAM, 0);
	if(listenfd<0)
	{	
		printf("socket creation failed\n");
		exit(20;
	}
	printf("Sockt established\n");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(MAX_port); //gotta check
	
	ret=bind(listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(ret<0)
	{
		printf("Failed to bind\n");
		exit(2);
		
	}
	if(listen(listenfd,10) === -1)
	{
	printf("listen fail\n");
	return -1;
	}
if(argc < 2)
{
	printf(Enter file name to be send:");
	gets(fname);
}
else

	strcpy(fname,argv[1]);
	while(1)
	{
		clen = sizeof(c_addr);
		printf("waiting.....\n");
		connectionfd = accept(listenfd, (struct sockaddr*)&c_addr, &clen);
		if(connectionfd<0)
		{
			printf("error in accepting\n ");
			continue;
		}
		err = pthread_create(&tid,NULL ,&SendFileToClient,&connfd);
		if(err !=0)
			printf("Cannot create thread:[%s],strerror(err));
			
	}
close(connectionfd);
retuen 0;
}

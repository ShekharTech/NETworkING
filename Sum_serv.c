#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<signal.h>
#include<arpa/inet.h>

void ohKillMe(int signo){
	raise(SIGKILL);
}

int doSum(int temp_num){
	int sum=0,digit=0;
	
	while(temp_num != 0){
		digit = temp_num % 10;
		sum = sum + digit;
		temp_num = temp_num / 10;
	}
	return sum;
}

void str_echo(int sockfd) {
	ssize_t n;
	char buf1[100],buf2[100];
	int num=0,sum=0;
	
	again:
	bzero(buf1, 100);
	while ( (n = read(sockfd, buf1, sizeof(buf1))) > 0){
			printf("Server Running........");
			//fputs(buf1,stdout);
			buf1[n]=0;
			num = atoi(buf1);
			sum = doSum(num);
			sprintf(buf2,"%d",sum);
			//buf2[n]=0;
			write(sockfd, buf2, strlen(buf2));
	}
		
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0){
		printf("Error");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	signal(SIGINT,ohKillMe);
	int listenfd,connfd,clen;
	struct sockaddr_in cliAddr, serAdd;
	pid_t childPid;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&serAdd, sizeof(serAdd));
	
	serAdd.sin_family = AF_INET;
	serAdd.sin_port = htons(8010);
	serAdd.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listenfd, (struct sockaddr*)&serAdd, sizeof(serAdd));

	listen(listenfd,5);
	while(1)
	{
		printf("Server started !!!!\n");
		clen = sizeof(cliAddr);
		connfd= accept(listenfd, (struct sockaddr*) &cliAddr, &clen);
		
		if((childPid = fork()) == 0)
		{
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}	
}

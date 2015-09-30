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

int checkNoOfDigit(int s){
	int counter=0;
	
	while(s!=0){
		s/=10;
		++counter;
	}
	return counter;
}

void str_cli(FILE *fp, int sockfd){
	char send[100],send1[100], recvi[100];
	int sum=0;

	while(1){
		printf("\nEnter the Number : ");
		while(fgets(send, sizeof(send), fp) != NULL){
			int n,i=0;	
			while(i<strlen(send)){
				if(isalpha(send[i])){
					printf("\nERROR : Invalid String\n");
					break;
				}
				i++;
			}
			
			if(i==strlen(send)){
				loop:
				write(sockfd, send, strlen(send));				
				fflush(stdout);
				if((n = read(sockfd, recvi, sizeof(recvi))) == 0){
					printf("error");
					exit(1);
				}
	
				recvi[n] = 0;
				printf("\nFrom Server : ");
				fputs(recvi, stdout);	
				sum = atoi(recvi);
				if((checkNoOfDigit(sum))==1){
					goto exit;
				}
				else{
					sprintf(send,"%d",sum);
					goto loop;
				}						
			}
			exit:
			break;
		}	
	}
}	

int main(int argc, char **argv){
	signal(SIGINT,ohKillMe);
	int sockfd;
	struct sockaddr_in servAdd;
	
	if(argc != 2){
		printf("\n usage : ./a.out<IPadd>");
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket");
		exit(1);
	}
		
	bzero(&servAdd, sizeof(servAdd));
	
	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(8010);
	
	inet_pton(AF_INET, argv[1], &servAdd.sin_addr);
	
	connect(sockfd, (struct sockaddr*)&servAdd, sizeof(servAdd));
	perror("Connect : ");
	str_cli(stdin, sockfd);
	exit(0);
}

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/poll.h>
#include<unistd.h>
#include<stdbool.h>


#define MAXLINE 1024
#define MAX_SOCK 512

char *esc = "exit\n";
int readline(int, char *, int);

struct profile{ 
	char Name[20];
	int len;
} chat_usr;

int main(int argc, char *argv[])
{
	char buf[MAXLINE] = {'\0'};
	char line[MAXLINE], sendmsg[MAXLINE];
	char time_buf[257];
	int sd, n, size, nread;
	int pid = (int)getpid();
	bool SHUTDOWN = false;

	struct sockaddr_in server_addr;

	struct pollfd job[2];

	//time 
	struct tm *tm;
	time_t t;
	//printf("%s\n",time_buf)



	if(argc < 4) {
		// printf("Wrong Usage.");
		printf("Usage: %s SERVER_ADDRESS TCP_PORT My_name\n",argv[0]);
		exit(1);
	}

	sprintf(chat_usr.Name,"%s", argv[3]);
	chat_usr.len = strlen(chat_usr.Name);


	// sockaddr_in 구조체 정의
	memset((char *)&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr =  inet_addr(argv[1]);


	// id definition
	char id[chat_usr.len + 4];
	char id_check[20];

	// socket 정의
	if (( sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){   //return sd when success, or -1
		printf("Client : Can't open stream socket.\n");
		exit(1);
	}

	while(1){
		// connect 
		if((connect(sd,(struct sockaddr *)&server_addr,sizeof(server_addr))) == -1){
			printf("Client : Can't connect to server.\n");
			close(sd);
			exit(1);
		} 

		printf("Enrolling ID...\n");

		// id send & check uniqueness.

		sprintf(id,"/id %s",chat_usr.Name);
		printf("%s\n",id);
		id[strlen(id)] = '\0';



		send(sd,id,strlen(id)+1, 0);

		if(recv(sd,id_check,20,0) < 0) {
			perror("ID_recv : No response from the server");
			close(sd);
			exit(1);
		}
		printf("%s\n", id_check);
		if(strncmp(id_check,"yes",3) == 0){
			printf("ID check Success.\n");
			break;
		}

		close(sd);
		printf("이미 존재하는 ID입니다. 다시 시도하십시오... ");
		scanf("%s", chat_usr.Name);

	} //ID check & connect while end


	// chat function

	// pollfd 구조체에 소켓 지시자를 할당
	job[0].fd = STDIN_FILENO;
	job[0].events = POLLIN;

	job[1].fd = sd;
	job[1].events = POLLIN;

	while(1) {
		printf("poll while loop in*******************************\n");
		nread = poll(job,2,-1); //job 에 대해서 무한정 대기

		if(nread > 0){

			//printf("stop at the job[1] POLLIN CHECK\n");
			if(job[1].revents == POLLIN){

				printf("Job[1] poll event occur: %d\n", nread);
				printf("existing buf: %s\n",buf);
				n = recv(sd, buf, MAXLINE,0);     

				printf("recv: %d\n",n);
				printf("buf : %s\n",buf);

				if(n < 0){
					perror("recv");
					close(sd);
					exit(1);
				}
				else if (n == 0){ 
					//	  if(SHUTDOWN == false){
					//	    SHUTDOWN = true;
					//	    continue;
					//	  }
					perror("Server is disconnected");
					close(sd);
					exit(1);
				}
				else {
					printf("%s\n",buf);
				}

				// exit 명령어 확인 
			}

			if(job[0].revents & POLLIN){
				readline(0,sendmsg,MAXLINE);
				printf("Job[0] POLLIN event occur!\n");
				//printf("**********%s", sendmsg);
				//sprintf(line,"[%s] %s", chat_usr.Name, sendmsg);
				size = strlen(sendmsg);
				sendmsg[size] = '\0';
				send(sd, sendmsg, size + 1, 0);

				if(strncmp(sendmsg,buf,4) == 0){
					printf("Goodbye\n");
					close(sd);
					kill(pid, SIGQUIT);
				}

			}
		}
	} // end of while	
} // end of main






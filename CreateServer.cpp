#include "Server.h"

int CreateServer(int port) 
{
	int s;
	struct sockaddr_in server_addr;
	int serverlen = sizeof(sockaddr_in);
	s = socket(AF_INET, SOCK_STREAM, 0);                                    

	memset((char *)&server_addr, 0, sizeof(server_addr));                       
	server_addr.sin_family = AF_INET;                                           
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);                            
	server_addr.sin_port = htons(port);                                         

	if(bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0){
		cout<<"bind error"<<endl;
		return -1;
	}

	listen(s, 5);                                                   
	return s;                                                                   
} 


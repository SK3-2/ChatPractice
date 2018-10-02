#include<iostream>
#include<cstdio>
#include"Server_h.h"

using namespace std;

int main(int argc,char* argv[]){

  int port_in =  atoi(argv[1]);
  int create_Server(int);


  //server socket open 
  int sd;
  if ((sd = create_Server(port_in)) == -1){
    return -1;
  }
  cout<<"티맥스 대화방 초기화중..."<<endl;
  
  //PollManager 생성 및 Pollfd 배정
  PollManager PM(sd);

  // Client Manager 생성
  ClientManager CM_1(&PM);

  cout<<"티맥스 대화방 시작"<<endl;

  while(1){
    PM.do_Poll(); //do_POll안에 while 걸기
  }

  return 0;
}


int create_Server(int port) 
{
  int s;
  struct sockaddr_in server_addr;
  int serverlen = sizeof(sockaddr_in);
  s = socket(AF_INET, SOCK_STREAM, 0);                                    
  //CHECK(s >= 0);                                                              
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


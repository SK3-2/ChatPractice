#include<iostream>
#include<cstdio>
#include"Server.h"

using namespace std;

int main(int argc,char* argv[]){
  int port_in =  atoi(argv[1]);
  int CreateServer(int);

  //server socket open 
  int sd;
  if ((sd = CreateServer(port_in)) == -1) return -1;
  cout<<"티맥스 대화방 초기화중..."<<endl;
  //Message 생성
	Message Msg;
  //PollManager 생성 및 Pollfd 배정
  EventManager EM(sd,&Msg);

  // Client Manager 생성
  ClientManager CM_1(&EM);
  cout<<"티맥스 대화방 시작"<<endl;

  EM.do_Poll(); 
  return 0;
}

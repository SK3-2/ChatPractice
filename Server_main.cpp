#include<iostream>
#include<cstdio>
#include"Server_h.h"

using namespace std;

int main(int argc,char* argv[]){
  int port_in =  atoi(argv[1]);
  int create_Server(int);

  //server socket open 
  int sd;
  if ((sd = create_Server(port_in)) == -1) return -1;
  cout<<"티맥스 대화방 초기화중..."<<endl;
  //Message 생성
	Message Msg;
  //PollManager 생성 및 Pollfd 배정
  PollManager PM(sd,&Msg);

  // Client Manager 생성
  ClientManager CM_1(&PM);
  cout<<"티맥스 대화방 시작"<<endl;

  PM.do_Poll(); 
  return 0;
}

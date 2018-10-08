#include <iostream>    
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <stdio.h>
#include <cstdio>      
#include <cstring> //C++    
#include <string> //C
#include <unistd.h>    
#include <sys/poll.h>  
#include "Server.h"

#define BUFMAX 1024    
#define IDMAX 20       
#define CMMAX 20
using namespace std;

//ClientManager 생성자
ClientManager::ClientManager(EventManager* ptr_p){
  pmptr=ptr_p;
  pmptr->register_ClientManager(this);
}


// Respond to POLL
void ClientManager::register_ID(Message* Msg, int fd){
  int index = get_key_by_ID(Msg->getAskedID());
  if(index > 0) {
    strcpy(buftemp,"no");
    send(fd,buftemp,sizeof(buftemp),0);
    close(fd);
    cout<<"Submitted ID is already existed - denied."<<endl;
  }
  else {
    number++;
    pmptr->register_Pollfd(fd);
    CSession[Msg->getFromIndex()] = new ClientSession(Msg->getFromIndex(),fd,Msg->getAskedID());
    buf="yes";
    CSession[Msg->getFromIndex()]->sendMsg(buf);
    broadMsg(Msg);
  }
}

// Whisper Case
void ClientManager::whispMsg(Message* Msg){
  int index;
  string toID = Msg->getToID();   
  
  if ((index=get_key_by_ID(toID)) == -1){
    CSession[Msg->getFromIndex()]->sendMsg("There is no person with that ID.");
  }
  else{
    CSession[index]->sendMsg(Msg->get_MsgFrame(CSession[Msg->getFromIndex()]));
  }
}

// Broadcast Case
void ClientManager::broadMsg(Message* Msg){
  int index = Msg->getFromIndex();
  string buf = Msg->get_MsgFrame(CSession[Msg->getFromIndex()]);
  broadcast_Message(buf,index);
}

// Setting Case
void ClientManager::settingMsg(Message* Msg){
  string command = Msg->getCommand();
  if(command.compare(0,5,"color") == 0){
    CSession[Msg->getFromIndex()]->set_Color(stoi(Msg->getValue()));
  }
  //if you want to put more options, put here!
  //...
}

//Close Session
void ClientManager::close_Session(Message* Msg){
  int myIndex = Msg->getFromIndex();
  string mbuf = Msg->get_MsgFrame(CSession[Msg->getFromIndex()]);
  broadcast_Message(mbuf,myIndex);
  CSession[myIndex]->set_myID("");
  CSession[myIndex]->set_mysd(-1);
  delete CSession[myIndex];
  CSession[myIndex] = NULL;
  pmptr->close_Pollfd(myIndex);
  number--;
}

//전체채팅을 보내는 함수
void ClientManager:: broadcast_Message(string Message, int index) {
  int cnt=0; //count variable: number of sending message 
  for (int i=1; i<=MAXINST-1; i++) {
    if (number == 0) break;
    if ((CSession[i] != NULL) &&\
	(CSession[i]->get_myID().compare(CSession[index]->get_myID())!=0)) {
      cnt++;
      CSession[i]->sendMsg(Message);
      if (cnt == number-1) { // to compare send number with client number except my_index
	break;
      }
    }
  }
  return;
}

//찾으려는 id를 가지고 있는 CSession array의 index 반환
int ClientManager:: get_key_by_ID(string ID) {
  for (int i=1; i<=MAXINST-1; i++) {
    if (CSession[i] != NULL){
      if(CSession[i]->get_myID().compare(ID)==0) {
      return i;
    }
    }
  }
  return 0;
}



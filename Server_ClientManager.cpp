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
#include "Server_h.h"

#define BUFMAX 1024    
#define IDMAX 20       
#define CMMAX 20
using namespace std;

//ClientManager 생성자
ClientManager::ClientManager(){}
ClientManager::ClientManager(PollManager* ptr_t){
  pmptr=ptr_t;
  pmptr->register_ClientManager(this);
}


// Respond to POLL
void ClientManager::registerID(Message* Msg){
	if(isExistID()) {
		buf="@no";
		send(Msg->getfromSd(),buf,sizeof(buf),0);
	}
	else {
		pmptr->register_Pollfd(Msg);
		CSession[Msg->getfromIndex()] = new ClientSession(Msg->getfromIndex(),Msg->getfromSd());
		buf="@yes";
		CSession[Msg->getfromIndex()]->sendMsg(buf);
		broadcast_Message(Msg->get_MsgFrame(GREET),Msg->getfromIndex());
	}
}

//귓속말 buf에서 private ID extract
string ClientManager::get_private_message_ID(string msg) {
  int index = msg.find(" "); //////enter 수정
  if(index == -1) {
    index = msg.find("\n");
  }
  return msg.substr(1,index-1);
}

//처음 등록시 들어오는 buf로부터 registration ID extract
string ClientManager::get_registration_ID(string msg) {
  return msg.substr(4);
}

//Client가 들어올 때, 들어온 Client의 ID를 이용해 greeting message frame을 만듦
string ClientManager:: get_greeting_message_frame(int index) {
  string frame = "@[" + CSession[index]->get_myID() + "]님이 티맥스 대화방에 입장했습니다.";
  return frame;
}

//Client가 나갈 때, 나가는 Client의 ID를 이용해 bye message frame을 만듦
string ClientManager:: get_bye_message_frame(int index) {
  string frame = "@[" + CSession[index]->get_myID() + "]님이 티맥스 대화방을 나가셨습니다.";
  return frame;
}

//Client가 전체채팅을 할 때, Client의 ID를 이용해 braodcast message frame을 만듦
string ClientManager:: get_broadcast_message_frame(string convs, int index) {
  string nid = CSession[index]->get_myID();
  string frame = "[" + nid+ "]: ";
  frame.append(convs);
  return frame;
}

//Client가 귓속말을 할 때, Client의 ID를 이용해 private message frame을 만듦
string ClientManager:: get_private_message_frame(string buf, string private_message_ID, int index) {
  string DM = "[DM_";
  string myname = CSession[index]->get_myID();
  cout<<"get_private_message_frame, myname: "<<myname<<endl;
  string msg_convs = buf.substr(private_message_ID.length()+2);
  DM.append(myname).append("]: ").append(msg_convs);
  return DM;
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
    if ((CSession[i] != NULL) && (CSession[i]->get_myID().compare(ID)==0)) {
      return i;
    }
  }
  return 0;
}



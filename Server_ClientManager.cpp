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

//ClientSession 생성자 함수
ClientSession::ClientSession() : index(-1), mysd(-1){
	strcpy(buftemp,"");
};
ClientSession::ClientSession(int index_t, int sd) : index(index_t), mysd(sd){
	strcpy(buftemp,"");
};
ClientSession::ClientSession(int index_t, int sd, string myID_t) : index(index_t), mysd(sd) {
	myID=myID_t;
	strcpy(buftemp,"");
};
//ClientSession 소멸자 함
ClientSession::~ClientSession() {
	cout<<"소멸자 실행"<<endl;
}
//recvMsg
string ClientSession::recvMsg() {
	strcpy(buftemp,""); //buf 초기화
	cout<<"recvMsg stage1"<<endl;
	int ret = recv(this->mysd, buftemp, sizeof(buftemp), 0);
	return buftemp;
}
//sendMsg
int ClientSession::sendMsg(string buf) {
	int n;
	strcpy(buftemp,buf.c_str());
	cout<<"_sendMSG_buftemp_"<<buftemp<<endl;
	//cout<<sizeof(buftemp)<<endl;
	buftemp[BUFMAX-1]='\0';
	if((n = send(mysd,buftemp,sizeof(buftemp),0)) < buf.length()) {
		cout<<"Msg buffer is not fully sent!"<<endl;
		return -1;
	}
	strcpy(buftemp,"");
	return 0;
}
//ClientSession get ID
string ClientSession::get_myID() {
	return myID;
}
//ClientSession set ID
void ClientSession::set_myID(string ID) {
	this->myID=ID;
}
//ClientSession get sd
int ClientSession::get_mysd() {
	return mysd;
}
// ClientSession set sd
void ClientSession::set_mysd(int mysd_t) {
	this->mysd=mysd_t;
}

//ClientManager 생성자 함수
ClientManager::ClientManager(){
	CSession[0]= new ClientSession();
}
ClientManager::ClientManager(PollManager* ptr_t){
	CSession[0]= new ClientSession();
	pmptr=ptr_t;
	cout<<"CM 생성자 호출 완료"<<endl;
	cout<<CSession[0]->get_mysd()<<endl;
	cout<<CSession[0]->get_myID()<<endl;
}


// Respond to POLL
void ClientManager::respond_Poll(int my_index, int sd, int N){
	cout<<"respond_Poll executed "<<N<<endl;
	buf="";
	if (N==0) { // 등록시 sd를 CSession[0]에 등록
		cout<<"recv"<<endl;
		CSession[0]->set_mysd(sd);
		buf = CSession[0]->recvMsg();
	}
	else { // 대화말이 들어오면 CSession[index] recvMsg 호출
		cout<<"Conversation "<<N<<endl;
		buf = CSession[my_index]->recvMsg();
	}
	// ClientSession 종료, 나가기
	if (buf.empty() == 1){ //EOF가 NULL로 생각
		cout<<"ClientSession 종료"<<endl;
		broadcast_Message(get_bye_message_frame(my_index),my_index);
		CSession[my_index]->set_myID("");
		CSession[my_index]->set_mysd(-1);
		delete CSession[my_index];
		number--;
		CSession[my_index]=NULL;
		pmptr->close_Pollfd(my_index);
		return;
	}

	// id 등록 확인
	if (buf.compare(0,3,"/id") == 0){
		int n;
		//id 중복체크	
		cout<<"id check"<<endl;
		cout<<buf<<endl;
		if((n=get_key_by_ID(buf.substr(4))) != 0) {
			cout<<"no"<<endl;
			CSession[0]->sendMsg("no");//중복이면 no 전달
			CSession[0]->set_myID("");
			CSession[0]->set_mysd(-1);
			pmptr->close_Pollfd(my_index);//PollManager close socket 
			return;
		}
		cout<<"yes"<<endl;
		CSession[0]->sendMsg("yes");//중복이 아니면 yes 전달 / 아직 등록되지 않았으므로 
		//CSession[0]의 sd로 보냄
		CSession[my_index] = new ClientSession(my_index,sd,buf.substr(4));
		cout<<"id registration complete"<<endl;
		number++;
		return;
	}

	// 귓속말인지 확인
	if (buf.compare(0,1,"@") ==0){
		if ((private_message_ID = get_private_message_ID(buf))=="") {
			perror("error");
			return;
		}
		int p_key = get_key_by_ID(private_message_ID);
		//Csession send Msg수
		CSession[p_key]->sendMsg(get_private_message_frame(buf,private_message_ID,my_index)); 
		return;
	}

	cout<<"전체채팅"<<endl;
	broadcast_Message(get_broadcast_message_frame(buf,my_index),my_index);
}

//들어온 귓속말 buf에서 private ID extract
string ClientManager::get_private_message_ID(string msg) {
	int index = msg.find(" ");
	return msg.substr(1,index-1);
}

//처음 등록시 들어오는 buf로부터 registration ID extract
string ClientManager::get_registration_ID(string msg) {
	return msg.substr(4);
}

//CSession array의 마지막 주소 반환
ClientSession* ClientManager:: get_session_end() {
	return CSession[MAXINST-1];
}

//Client가 나갈 때, 나가는 Client의 ID를 이용해 bye message frame을 만듦
string ClientManager:: get_bye_message_frame(int index) {
	string frame = "[" + CSession[index]->get_myID() + "]님이 티맥스 대화방을 나가셨습니다.\n";
	cout<<"_get_bye_message_frame_"<<endl;
	return frame;
}

//Client가 전체채팅을 할 때, Client의 ID를 이용해 braodcast message frame을 만듦
string ClientManager:: get_broadcast_message_frame(string convs, int index) {
	string frame = "[" + CSession[index]->get_myID() + "] ";
	frame.append(convs);
	return frame;
}

//Client가 귓속말을 할 때, Client의 ID를 이용해 private message frame을 만듦
string ClientManager:: get_private_message_frame(string buf, string private_message_ID, int index) {
	string DM = "[DM] ";
	string myname = CSession[index]->get_myID();
	string msg_convs = buf.substr(private_message_ID.length()+2);
	DM.append(myname).append(": ").append(msg_convs);
	return DM;
}

//전체채팅을 보내는 함수
void ClientManager:: broadcast_Message(string Message, int index) {
	cout<<"_broadcast_Message_step"<<endl;

	int cnt=0; //send number count variable 
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
	//cout<<"_get_key_by_ID_"<<ID<<endl;
	for (int i=1; i<=MAXINST-1; i++) {
		if ((CSession[i] != NULL) && (CSession[i]->get_myID().compare(ID)==0)) {
			return i;
		}
	}
	return 0;
}



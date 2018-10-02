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

//ClientManager 생성자 함수
ClientManager::ClientManager(){
}
ClientManager::ClientManager(PollManager* ptr_t){
	pmptr=ptr_t;
	pmptr->register_ClientManager(this);
}


// Respond to POLL
void ClientManager::respond_Poll(int my_index, int sd, int N){
	buf="";//buf 초기화
	if (N==0) { // 등록시 sd를 CSession[index]에 등록 및 number++
		CSession[my_index] = new ClientSession(my_index,sd);
		buf = CSession[my_index]->recvMsg();
		number++;
	}
	else { // 대화말이 들어오면 CSession[index] recvMsg 호출
		buf = CSession[my_index]->recvMsg();
	}

	switch(int k=Parser(buf, this)) {
		case 0: {	//id 중복체크 후 중복이면 close socket	
							int n;
							if((n=get_key_by_ID(get_registration_ID(buf)) != 0)) {
								CSession[my_index]->sendMsg("@no");//중복이면 no 전달
								CSession[my_index]->set_myID("");
								CSession[my_index]->set_mysd(-1);
								delete CSession[my_index];
								CSession[my_index]=NULL;
								pmptr->close_Pollfd(my_index);//PollManager close socket 
								number--;
							}
							return;
							break;
						}
		case 1: {	//id 중복체크 후 중복이 아니면 CSession[index]에 ID 등록
							CSession[my_index]->set_myID(get_registration_ID(buf));
							CSession[my_index]->sendMsg("@yes");//중복이 아니면 yes 전달 
							return;
							break;
						}
		case 2: { //귓속말 보내기, 받을 상대(CSession[index])의
							//ID와 받을 상대(CSession[index])의 sd를 가져와 명령 수행
							if ((private_message_ID = get_private_message_ID(buf))=="") {
								perror("error");
								return;
							}
							int p_key = get_key_by_ID(private_message_ID);
							//cout<<"귓속말 index : "<<p_key<<endl;
							//Csession send Msg수
							CSession[p_key]->sendMsg(get_private_message_frame(buf,private_message_ID,my_index)); 
							return;
							break;
						}
		case 3: { //Client 종료, 종료메시지를 나머지 Client에게 전달하고,
							//close socket 및 number 감소
							broadcast_Message(get_bye_message_frame(my_index),my_index);
							CSession[my_index]->set_myID("");
							CSession[my_index]->set_mysd(-1);
							delete CSession[my_index];
							number--;
							CSession[my_index]=NULL;
							pmptr->close_Pollfd(my_index);
							return;
							break;
						}
		case 4: { //Client font color 바꾸기
							int C = atoi(buf.substr(7,2).c_str());
							CSession[my_index]->set_Color(C);
							return;
							break;
						}
		case 5: { //전체채팅 보내기, (CSession[my_index])의 ID와 buf를 가져와
							//나머지 CSession들의 sd로 sendMsg
							broadcast_Message(get_broadcast_message_frame(buf,my_index),my_index);
							return;
							break;
						}
		default:
						return;
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

//Client가 나갈 때, 나가는 Client의 ID를 이용해 bye message frame을 만듦
string ClientManager:: get_bye_message_frame(int index) {
	string frame = "@[" + CSession[index]->get_myID() + "]님이 티맥스 대화방을 나가셨습니다.\n";
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



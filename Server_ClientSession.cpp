#include "Server_h.h"

//ClientSession 생성자 함수

ClientSession::ClientSession() : index(-1), mysd(-1){strcpy(buftemp,"");};
ClientSession::ClientSession(int index_t, int sd) : index(index_t), mysd(sd){
  strcpy(buftemp,"");};
ClientSession::ClientSession(int index_t, int sd, string myID_t) : index(index_t), mysd(sd) {
	myID=myID_t;
	strcpy(buftemp,"");
};
//ClientSession 소멸자 함
ClientSession::~ClientSession() {
	cout<<"소멸자 실행"<<endl;
}


// color setting by ClientManager
void ClientSession::set_Color(int ncolor) { 
  color = ncolor;
  return;
}

int ClientSession::get_Color() {
	return this->color;
}

string ClientSession::get_FontFrame(int color)
{
  string cbuf = "\33[";
  cbuf += to_string(color);
  cbuf += "m";
  return cbuf;
}


//recvMsg
//append buftemp with color command
string ClientSession::recvMsg() {
	strcpy(buftemp,""); //buf 초기화
	cout<<"recvMsg stage"<<endl;
	int ret = recv(this->mysd, buftemp, sizeof(buftemp), 0);

	if(buftemp[0] == '/') return buftemp;
	else if(buftemp[0] == '\0') return buftemp;
	
	return buftemp+get_FontFrame(this->color);
}

//sendMsg
int ClientSession::sendMsg(string buf) {
	int n;
	int size = buf.length();
	// CM에서 @ 로 보내주는 buf는 @ 를 빼고 나서 특수처리
	if(buf.compare(0,1,"@")==0){
	  buf = buf.substr(1);
	}
	else if(!buf.empty()){
	//put colorbuf in front of Msgbuf
	  string colorbuf = buf.substr(size-5);
	  buf = colorbuf + buf.substr(0,size-6);
	  buf += get_FontFrame(this->color);
	  //cout<<"final buf: "<<buf<<"///"<<endl;
	}
	
	strcpy(buftemp,buf.c_str());
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
	this->mysd = mysd_t;
}

#include "Server.h"

//ClientSession 생성자 함수
ClientSession::ClientSession() : index(-1), mysd(-1){strcpy(buftemp,"");};
ClientSession::ClientSession(int index_t, int sd) : index(index_t), mysd(sd){strcpy(buftemp,"");};
ClientSession::ClientSession(int index_t, int sd, string myID_t) : index(index_t), mysd(sd) {
  myID=myID_t;
  strcpy(buftemp,"");
};

//ClientSession 소멸자 함
ClientSession::~ClientSession() {
  cout<<"ClientSession closed"<<endl;
}


// color setting by ClientManager
void ClientSession::set_Color(int ncolor){ 
  this->color = ncolor;
  string colorSet = get_FontFrame()+"User color is successfully changed";
  int nc = send(this->mysd,colorSet.c_str(),colorSet.length()+1,0);
  return;
}

int ClientSession::get_Color() {
	return this->color;
}

string ClientSession::get_FontFrame()
{
  string cbuf = "\33[";
  cbuf += to_string(this->color);
  cbuf += "m";
  return cbuf;
}

//sendMsg
int ClientSession::sendMsg(string buf) {
    
  int n;
  int size = buf.length();


  //put colorbuf in front of Msgbuf
  if(!buf.empty()){
    buf += get_FontFrame();
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

#include "Server.h"

//Put recv Msg&Info  -- used in Receiving Object
void Message::set_Msg(int index, string buf){
  fromIndex = index;
  cout<<"setMsg: "<<buf<<endl;
  msgBuffer = buf;
  mtype = parseMsg(msgBuffer);
  return;
}

Message::MsgType Message::parseMsg(string buf){
  MsgType type;
  if(buf.compare(0,1,"/")==0){
    if(buf.compare(1,2,"id")==0)
      type = MsgType::GREET;   
    else 
      type = MsgType::SET;  //SET인 경우는, Command를 추가로 비교
  }
  else if(msgBuffer.compare(0,1,"@")==0)
    type = MsgType::WHISP;
  else if(msgBuffer.empty()==1) {
    cout<<"Msg is empty"<<endl;
    type = MsgType::BYE;
  }
  else 
    type = MsgType::BROAD;
  return type;
}

//Ask by isCase
bool Message::isSetID() const {
  if(mtype == MsgType::GREET)
    return true;
  return false;
}
bool Message::isWhisper() const {
  if(mtype == MsgType::WHISP)
    return true;
  return false;
}
bool Message::isSetting() const {
  if(mtype == MsgType::SET)
    return true;
  return false;
}
bool Message::isEmpty() const {
  if(mtype == MsgType::BYE)
    return true;
  return false;
}

//Throw Token
string Message::getToID(){
  return tokenMsg(msgBuffer,1);
}
string Message::getAskedID(){
  fromID = tokenMsg(msgBuffer,2);
  return fromID;
}

int Message::getFromIndex(){
  return fromIndex;
}
string Message::getCommand(){
  return tokenMsg(msgBuffer,1);
}
string Message::getValue(){
  return tokenMsg(msgBuffer,2);
}

//Tokenize Function
string Message::tokenMsg(string buf,int order){
  int cur = 0; //Search Starts from Index 0
  int head; //to erase a command character.
  if(buf.empty()==1){
    return "";
  }

  for(int i=0; i<order; i++){
    head = cur;
    cur = buf.find(" ",cur);
    if(cur == -1){
      // 방어코드가 필요함, 원하는 token 개수 만큼 안 들어왔을 때.
      cur = buf.find("\n");
      break;
    }
  }
  return buf.substr(head+1,cur-head-1);
}

//Revise the Msg to ID contained Format
string Message::get_MsgFrame(ClientSession* csptr){
  fromID = csptr->get_myID();
  fromColor = csptr->get_Color();

  if(mtype == MsgType::GREET){
    msgBuffer = "\33[39m[" + fromID + "] enters to the Chat.";
  }
  else if(mtype == MsgType::BYE){
    msgBuffer = "\33[39m[" + fromID + "] exits from the Chat.";
  }
  else if(mtype == MsgType::WHISP){
    msgBuffer = csptr->get_FontFrame() + "[DM_" + fromID + "] " + msgBuffer;
  }
  else if(mtype == MsgType::BROAD){
    msgBuffer = csptr->get_FontFrame() + "[" + fromID + "] " + msgBuffer;
  }
  else{
    msgBuffer = "";
  }
  return msgBuffer;
}

void Message::clear(){
  fromSd = -1;
  fromID = "";
  fromIndex = -1;
  msgBuffer = "";
}

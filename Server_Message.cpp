#include "Server_h.h"

//Put recv Msg&Info  -- used in Receiving Object
void Message::set_Msg(int sd, int index, string buf){
	fromSd = sd;
	fromIndex = index;
	msgBuffer = buf;	
	return;
}

//Ask by isCase
bool Message::isSETID(){
	if(msgBuffer.compare(0,3,"/id")==0) return true;
	mtype = MsgType::GREET;
	return false;
}
bool Message::isWhisper(){
	if(msgBuffer.compare(0,1,"@")==0) return true;
	mtype = MsgType::WHISP;
	return false;
}
bool Message::isSetting(){
	if(msgBuffer.compare(0,6,"/color")==0) return true;
	return false;
}
bool Message::isEmpty(){
	if(msgBuffer.empty()==1) return true;
	mtype = MsgType::BYE;
	return false;
}

//Throw Token
string Message::getToID(){
	return tokenMsg(msgBuffer,1);
}
string Message::getFromID(){
	fromID = tokenMsg(msgBuffer,2);
	return fromID;
}
string Message::getCommand(){
	return tokenMsg(msgBuffer,1);
}
string Message::getColor(){
	return tokenMsg(msgBuffer,2);
}

//Tokenize Function
string Message::tokenMsg(string buf,int order){
	int cur = 0; //Search Starts from Index 0
	int head = 1; //to erase a command character.

	for(int i=0; i<order; i++){
		cur = buf.find(" ",cur);
		if(cur == -1){
			// 방어코드가 필요함, 원하는 token 개수 만큼 안 들어왔을 때.
			cur = buf.find("\n");
			break;
		}
		head = cur;
	}
	return buf.substr(head,cur-1);
}

//Revise the Msg to ID contained Format
string Message::get_MsgFrame(){
	if(mtype == MsgType::GREET){
		msgBuffer = "[" + fromID + "] enters to the Chat.";
	}
	else if(mtype == MsgType::BYE){
		msgBuffer = "[" + fromID + "] exits from the Chat.";
	}
	else if(mtype == MsgType::WHISP){
		msgBuffer = "[DM_" + fromID + "] " + msgBuffer;
	}
	else if(mtype == MsgType::BROAD){
		msgBuffer = "[" + fromID + "] " + msgBuffer;
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

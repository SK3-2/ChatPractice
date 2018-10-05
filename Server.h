#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/poll.h>

#define BUFMAX 1024
#define MAXINST 512
#define IDMAX 20
#define CMMAX 20

using namespace std;

class PollManager;
class ClientManager;
class ClientSession;


class ClientSession{              
  private:
    int index;
    int mysd;
    string myID="";
    int color = 39; //default(white)    
    string get_FontFrame(int);	
  public:
    ClientSession();
    ~ClientSession();
    void set_Color(int);
    int get_Color();
    ClientSession(int, int);
    ClientSession(int, int, string);
    void set_mysd(int);
    int get_mysd();
    void set_myID(string);
    string get_myID();
    int sendMsg(string);
};                                


class ClientManager{                              
  private:                                        
    PollManager* pmptr;                         
    ClientSession* CSession[MAXINST];           
    int number=0;
    string buf;                                    
    string private_message_ID;                    

    void close_ClientSession();                    
    void broadcast_Message(string, int); 
    string get_private_message_ID(string); 
    string get_greeting_message_frame(int);
    string get_bye_message_frame(int);  
    string get_broadcast_message_frame(string, int); 
    string get_private_message_frame(string, string, int); 

  public:                                         
    ClientManager();
    ClientManager(PollManager*);  //used by main                 
    void respond_Poll(int, int, int);    // used by PM              
    string get_registration_ID(string);  //used by parser
    int get_key_by_ID(string);    //used by parser                

};                                                


class PollManager{
  private:
    char buftemp[BUFMAX];
    struct pollfd g_pollfd[MAXINST];  
    ClientManager* cmptr;
    int nread = 0;
    int serverfd;
    const struct pollfd* pollfd_end = &g_pollfd[MAXINST-1];

    int get_EmptyPfdIndex(void);
    int accept_Pollfd(int);
    pollfd* get_NextPollfd(pollfd*);
    pollfd* get_NextReventPoll(pollfd*);
    string recvMsg(int);
  public:
    PollManager(int); 
    void do_Poll(); //used by main
    void close_Pollfd(int); //used by CM
    int register_ClientManager(ClientManager*);  
};

class Message{
  private:
    enum class MsgType {GREET,BYE,WHISP,BROAD,EMPTY};
    MsgType mtype = EMPTY; 

    int fromSd;
    int fromIndex;
    string msgBuffer;
    string fromID;
    string tokenMsg(string, int);
  public:	
    void set_Msg(n*,string);
    bool isSetID();
    bool isWhisper();
    bool isEmpty();
    string getToID();
    string getFromID();
    string getColor();
    void clear();
    string get_MsgBuf();
    string set_MsgFrame(string, MsgType);
};

//Put recv Msg&Info  -- used in Receiving Object
void Message::set_Msg(int sd, int index, string buf){
  fromSd = sd;
  fromIndex = index;
  msgBuffer = buf;
  return;
}

//Ask by isCase 
bool Message::isWhisper(){
  if(buf.compare(0,1,"@")==0) return true;
  return false;
}
bool Message::isSetting(){
  if(buf.compare(0,6,"/color")==0) return true;
  return false;
}
bool Message::isEmpty(){
  if(buf.emtpy()==1) return true;
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
      cur = msg.find("\n");
      break;
    }
    head = cur;
  }
  return msg.substr(head,cur-1);
}

//Revise the Msg to ID contained Format
string Message::set_MsgFrame(string buf, MsgType mtype){
  if(mtype == GREET){
    buf = "[" + fromID + "] enters to the Chat.";
  }                                                                      
  else if(mtype == BYE){
    buf = "[" + fromID + "] exits from the Chat."
  }
  else if(mtype == WHISP){
    buf = "[DM_" + fromID + "] " + buf;
  }
  else if(mtype == BROAD){
    buf = "[" + fromID + "] " + buf;
  }
  else{
    return "";
  }
  return buf;
}

void Message::clear(){
    from = NULL;
    fromID = "";
    msgBuffer = "";
}

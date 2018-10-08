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
#define MAXINST 100 
#define IDMAX 20
#define CMMAX 20

using namespace std;

class EventManager;
class ClientManager;
class ClientSession;
class Message;


class ClientSession{              
	private:
		int index;
		int mysd;
		char buftemp[BUFMAX];
		string myID="";
		int color = 39; //default(white)    
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
		string get_FontFrame();	
};                                


class ClientManager{                              
	private:                                        
		EventManager* pmptr;                         
		ClientSession* CSession[MAXINST] = {NULL};           
		int number=0;
		string buf;                                    
		char buftemp[BUFMAX];
		string private_message_ID;                    
		int get_key_by_ID(string);    //used by parser                

	public:                                         
		ClientManager();
		ClientManager(EventManager*);  //used by main                 
		void register_ID(Message*,int);
		void whispMsg(Message*);
		void settingMsg(Message*);
		void broadMsg(Message*);
		void broadcast_Message(string, int); 
		void close_Session(Message*);                    

};                                                


class EventManager{
	private:
		char buftemp[BUFMAX];
		struct pollfd g_pollfd[MAXINST];  
		ClientManager* cmptr;
		Message* mptr;
		int nread = 0;
		int serverfd;
		const struct pollfd* pollfd_end = &g_pollfd[MAXINST-1];

		int get_EmptyPfdIndex(void);
		int accept_Pollfd(int);
		pollfd* get_NextPollfd(pollfd*);
		pollfd* get_NextReventPoll(pollfd*);
		string recvMsg(int);

	public:
		EventManager(int, Message*); 
		void do_Poll(); //used by main
		void close_Pollfd(int); //used by CM
		void register_Pollfd(int);
		int register_ClientManager(ClientManager*);  
};


class Message{
	private:
		enum class MsgType {GREET,BYE,WHISP,BROAD,EMPTY,SET};
		MsgType mtype;

		int fromSd;
		int fromIndex;
		int fromColor;
		string msgBuffer;
		string fromID;
		string tokenMsg(string, int);
		Message::MsgType parseMsg(string buf);
	public:    
		void set_Msg(int,string);
		bool isSetting() const;
		bool isSetID() const;
		bool isWhisper() const;
		bool isEmpty() const;
		
		string getToID();
		string getAskedID();
		
		int getFromIndex();
		
		string getCommand();
		string getValue();
		
		void clear();
		string get_MsgBuf();
		string get_MsgFrame(ClientSession*);
		
		void setFromID(string);
};



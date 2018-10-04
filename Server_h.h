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
    char buftemp[BUFMAX];
    int color = 39;
    
  public:
    ClientSession();
    ~ClientSession();
    void set_Color(int);
		int get_Color();
    ClientSession(int, int);
    ClientSession(int, int, string);
    string recvMsg();
    int sendMsg(string);
    string get_FontFrame(int);
    string get_myID();
		void set_myID(string);
    int get_mysd();
		void set_mysd(int);
};                                


class ClientManager{                              
  private:                                        
    PollManager* pmptr;                           
	public:
		int number=0;
    ClientSession* CSession[MAXINST];             
    string buf;                                    
    string private_message_ID;                    
  public:                                         
    ClientManager();                              
    ClientManager(PollManager*);                  
    void respond_Poll(int, int, int);                  
    void close_ClientSession();                   
		void broadcast_Message(string, int);
    string get_private_message_ID(string);  
		string get_registration_ID(string);
    int get_key_by_ID(string);                    
		string get_bye_message_frame(int);
		string get_broadcast_message_frame(string, int);
		string get_private_message_frame(string, string, int);
};                                                


class PollManager{
  private:
    struct pollfd g_pollfd[MAXINST];  
    int g_pollfd_cmindex[MAXINST] = {-1};
    ClientManager* cmptr;

    int nread = 0;
    int serverfd;
    int events = POLLIN;
    const struct pollfd* pollfd_end = &g_pollfd[MAXINST-1];

  public:
    PollManager(int);
    int register_ClientManager(ClientManager*);  
    void do_Poll(); //public
    int accept_Pollfd(int);
    int get_EmptyPfdIndex(void);
    pollfd* get_NextPollfd(pollfd*);
    pollfd* get_NextReventPoll(pollfd*);
    void close_Pollfd(int); //public
};

int Parser(string, ClientManager*);

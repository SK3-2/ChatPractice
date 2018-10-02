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
    int color = 39; //default(white)    
    string get_FontFrame(int);
  public:
    ClientSession();
    ClientSession(int, int);
    ClientSession(int, int, string);
    ~ClientSession();
    void set_Color(int);
    void set_mysd(int);
    int get_mysd();
    void set_myID(string);
    string get_myID();
    string recvMsg();
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
    string get_bye_message_frame(int);  
    string get_broadcast_message_frame(string, int); 
    string get_private_message_frame(string, string, int); 

  public:                                         
    ClientManager(PollManager*);  //used by main                 
    void respond_Poll(int, int, int);    // used by PM              
    string get_registration_ID(string);  //used by parser
    int get_key_by_ID(string);    //used by parser                
};                                                


class PollManager{
  private:
    struct pollfd g_pollfd[MAXINST];  
    int g_pollfd_cmindex[MAXINST] = {-1};
    ClientManager* cmptr;
    int nread = 0;
    int serverfd;
    const struct pollfd* pollfd_end = &g_pollfd[MAXINST-1];

    int register_ClientManager(ClientManager*);  
    int get_EmptyPfdIndex(void);
    int accept_Pollfd(int);
    pollfd* get_NextPollfd(pollfd*);
    pollfd* get_NextReventPoll(pollfd*);

  public:
    PollManager(int); 
    void do_Poll(); //used by main
    void close_Pollfd(int); //used by CM
};

int Parser(string, ClientManager*);

#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/poll.h>
#include"Server.h"

using namespace std;

//생성자
EventManager::EventManager(int sd, Message* ptr): serverfd(sd){
  mptr = ptr;
  for(int i=0; i<MAXINST; i++){
    g_pollfd[i] = {-1,POLLIN, 0};
  }
  g_pollfd[0].fd = serverfd;
  return;
}

// Client Manager를 등록함: 0 = 성공, -1 = 실패
int EventManager::register_ClientManager(ClientManager* ptr){
  if(ptr == NULL) return -1;
  cmptr = (ClientManager*)ptr;
  return 0;
}

void EventManager::register_Pollfd(int nfd) {
  g_pollfd[mptr->getFromIndex()].fd = nfd;
}

// Get Next assigned pollfd Ptr
struct pollfd* EventManager::get_NextPollfd(struct pollfd* cur){
  int index;
  if(cur==NULL) index = 0;
  else{
    index = cur - &g_pollfd[0];
    index++;
  }
  //Search pfd which has fd not equal to -1
  struct pollfd* pfd;
  for(pfd = &g_pollfd[index]; pfd < pollfd_end;++pfd){
    if(pfd->fd != -1) return pfd;
  }
  return NULL;
}

// Poll revent handler
struct pollfd* EventManager::get_NextReventPoll(struct pollfd* pfd){
  struct pollfd* cur = pfd;
  while((cur = get_NextPollfd(cur))){
    if(cur->revents == POLLIN){
      return cur;
    }				
  }
  return NULL;
}  

// Get Empty pollfd index
int EventManager::get_EmptyPfdIndex(void){
  for (struct pollfd* pfd = &g_pollfd[0]; pfd < pollfd_end; pfd++){
    if(pfd->fd == -1){
      return (int)(pfd - &g_pollfd[0]);
    }
  }
  return -1;
}      

// Accept pollfd
int EventManager::accept_Pollfd(int sd){
  struct sockaddr_in ADDR_IN;
  int serverlen = sizeof(sockaddr_in);
  int nfd = accept(sd, (struct sockaddr *)&ADDR_IN, (socklen_t *)&serverlen);
  return nfd;
}


string EventManager::recvMsg(int fd) {
  strcpy(buftemp,""); //buf 초기화
  int ret = recv(fd, buftemp, sizeof(buftemp), 0);
  return buftemp;
}


// Execute Poll 
void EventManager::do_Poll(void){
  while(1){
    nread = poll(g_pollfd, MAXINST, -1);

    struct pollfd* cur = NULL;    
    for (int n_poll = 0; n_poll < nread; n_poll++) {  // iterate nread times
      cur = get_NextReventPoll(cur);  // return type : struct pollfd*
      int index = cur-&g_pollfd[0];

      if(index == 0){
	cout<<"Accept Socket Event Occur!"<<endl;
	int nfd = accept_Pollfd(serverfd);	
	int EmptyPfdIndex = get_EmptyPfdIndex();
	mptr->set_Msg(EmptyPfdIndex,recvMsg(nfd));
	if(mptr->isSetID()) {
	  cmptr->register_ID(mptr,nfd);
	}
	else cout<<"Wrong ID Submission"<<endl;
      }
      else if(index > 0){
	cout<<"Client Socket Event Occur!"<<endl;
	int csd = g_pollfd[index].fd;

	mptr->set_Msg(index,recvMsg(csd));

	if(mptr->isSetting()){
	  cmptr->settingMsg(mptr);	
	}
	else if(mptr->isEmpty()){
	  cmptr->close_Session(mptr);
	}
	else if(mptr->isWhisper()){
	  cmptr->whispMsg(mptr);	
	}
	else{
	  cmptr->broadMsg(mptr);
	}

      }
      else{
	cout<<"Wrong Index Input: "<<cur->fd<<endl;      
      }
    }
  }
}

// close Poll
void EventManager::close_Pollfd(int index){
  close(g_pollfd[index].fd); 
  g_pollfd[index].fd  = -1;
  g_pollfd[index].revents = 0;
}


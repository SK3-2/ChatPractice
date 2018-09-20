#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/poll.h>
#include"Server_h.h"

using namespace std;


//생성자
PollManager::PollManager(int sd): serverfd(sd)
{
  for(int i=0; i<MAXINST; i++){
    g_pollfd[i] = {-1,POLLIN, 0};
  }
  g_pollfd[0].fd = serverfd;
  return;
}



// Client Manager를 등록함: 0 = 성공, -1 = 실패
int PollManager::register_ClientManager(ClientManager* ptr){
  if(ptr == NULL)
    return -1;
  cmptr = (ClientManager*)ptr;
// cout<<"CM 등록"<<endl;
//  cout<<cmptr->CSession[0]->get_mysd()<<endl;
//  cout<<cmptr->CSession[0]->get_myID()<<endl;	
  return 0;
}


// Get Next pollfd Ptr
struct pollfd* PollManager::get_NextPollfd(struct pollfd* cur){
  int index;
  if(cur==NULL) index = 0;
  else{
    index = cur - &g_pollfd[0];
    index++;
  }

  struct pollfd* pfd;
  //cout<<"Next pollfd index = "<<index<<endl;


  for(pfd = &g_pollfd[index]; pfd < pollfd_end;++pfd)
  {
   //cout<<"index = "<<pfd-&g_pollfd[index]<<endl;
    if(pfd->fd != -1) return pfd;
  }
  return NULL;

}

// Poll revent handler
struct pollfd* PollManager::get_NextReventPoll(struct pollfd* pfd){
  struct pollfd* cur = pfd;
  while((cur = get_NextPollfd(cur))){
    //cout<<"while in"<<endl;
    if(cur->revents == POLLIN){
      return cur;
    }				
  }
  return NULL;
}  

// Get Empty pollfd index
int PollManager::get_EmptyPfdIndex(void){
  for (struct pollfd* pfd = &g_pollfd[0]; pfd < pollfd_end; pfd++){
    if(pfd->fd == -1){
      return (int)(pfd - &g_pollfd[0]);
    }
  }
  return -1;
}      

// Accept pollfd
int PollManager::accept_Pollfd(int sd){
  struct sockaddr_in ADDR_IN;
  int serverlen = sizeof(sockaddr_in);
  int nfd = accept(sd, (struct sockaddr *)&ADDR_IN, (socklen_t *)&serverlen);
  return nfd;
}


// Execute Poll 
void PollManager::do_Poll(void){
  struct pollfd* cur = NULL;   
  nread = poll(g_pollfd, MAXINST, -1); //poll
  //cout<<"nread = "<<nread<<endl;
  for (int n_poll = 0; n_poll < nread; n_poll++)  // iterate nread times
  { 
    //cout<<"poll loop start"<<endl;

    cur = get_NextReventPoll(cur);  // return type : struct pollfd*
    int index = cur - &g_pollfd[0];
    //cout<<"index = "<<index<<endl;
    if(index == 0)
    {
      cout<<"Accept Socket Event Occur!"<<endl;
      int EmptyPfdIndex = get_EmptyPfdIndex();
      int nfd = accept_Pollfd(serverfd);	 
      g_pollfd[EmptyPfdIndex].fd = nfd; // Save new client's sd in emptypfdindex	 
      cmptr->respond_Poll(EmptyPfdIndex, nfd, 0);
    } // 
    else if(index > 0){
      cout<<"Client Socket Event Occur!"<<endl;
      cmptr->respond_Poll(index,cur->fd,1); 
    }
    else{
      cout<<"Wrong Index Input Error: sd "<<cur->fd<<endl;      
    }
  }
}

// close Poll
void PollManager::close_Pollfd(int index){
  close(g_pollfd[index].fd);
  g_pollfd[index].fd  = -1;
  g_pollfd[index].revents = 0;
}



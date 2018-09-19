#include "Server_h.h"

int Parser(string buf,ClientManager* cmptr) {
	int n;
	// id 등록 요청 확인 후, id 중복이면 return 0 
	if (buf.compare(0,3,"/id") == 0 && (n=cmptr->get_key_by_ID(buf.substr(4))) != 0) {   
			return 0;                                                                      
	}         
	// id 등록 요청 확인 후, id 중복이면 return 1
	if(buf.compare(0,3,"/id") == 0 && (n=cmptr->get_key_by_ID(buf.substr(4))) == 0) {           
			return 1;                                                                       
	}
	// 귓속말 요청일 경우, return 2
	if (buf.compare(0,1,"@") ==0) {                                                         
		return 2;                                                                        
	}                                                                                      
	// ClientSession 종료 요청일 경우, return 3                                                    
	if (buf.empty() == 1) {                                       
		return 3;                                                                        
	}                                                                                
	// 전체채팅 요청일 경우, return 4
	return 4;
}

#include "Server_h.h"

int Parser(string buf,ClientManager* cmptr) {
	int n;
	// id 중복                                                                  
	if (buf.compare(0,3,"/id") == 0 && (n=cmptr->get_key_by_ID(buf.substr(4))) != 0) {   
			return 0;                                                                      
	}         
	// id 중복 아님
	if(buf.compare(0,3,"/id") == 0 && (n=cmptr->get_key_by_ID(buf.substr(4))) == 0) {           
			return 1;                                                                       
	}
	// 귓속말
	if (buf.compare(0,1,"@") ==0) {                                                         
		return 2;                                                                        
	}                                                                                      
	// ClientSession 종료, 나가기                                                    
	if (buf.empty() == 1) {                                       
		return 3;                                                                        
	}                                                                                
	// 전체채팅
	return 4;
}

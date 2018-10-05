server:
	g++ -o Server Server_ClientManager.cpp Server_PollManager.cpp Server_main.cpp Server_createServer.cpp Server_h.h Server_ClientSession.cpp Server_Message.cpp
client:
	gcc -o Client Client.c readline.c
all: server client

clean:
	rm Server Client


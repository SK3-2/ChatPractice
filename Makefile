server:
	g++ -o Server ClientManager.cpp EventManager.cpp main.cpp CreateServer.cpp Server.h ClientSession.cpp Message.cpp
client:
	gcc -o Client Client.c readline.c
all: server client

clean:
	rm Server Client


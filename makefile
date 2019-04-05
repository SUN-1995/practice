all:server_epoll client_epoll clean_out
server_epoll:server_epoll.o
	g++ -g -o server_epoll server_epoll.o
client_epoll:client_epoll.o
	g++ -g -o client_epoll client_epoll.o
server_epoll.o:server_epoll.cpp
	g++ -g -c server_epoll.cpp
client_epoll.o:client_epoll.cpp
	g++ -g -c client_epoll.cpp
clean_out:
	rm server_epoll.o client_epoll.o
clean:
	rm server_epoll client_epoll
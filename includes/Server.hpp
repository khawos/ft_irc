#pragma once
# include <sys/socket.h> // socket() listen()
# include <iostream> // cout cerr
# include <unistd.h> // close()
# include <poll.h> // poll()
# include <netinet/in.h> // struct sockaddr
# include <string.h> // memset
# include <vector>
class Server {

    private:

        int _socketFD;
        int _port;
	    std::vector<pollfd> _pollFds;
    public:

        Server();
        ~Server();

    void    runServer();
};
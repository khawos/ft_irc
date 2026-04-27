#pragma once
# include <sys/socket.h> // socket() listen()
# include <iostream> // cout cerr
# include <unistd.h> // close()
# include <fcntl.h> // fcntl()
# include <poll.h> // poll() - struct pollfd
# include <netinet/in.h> // struct sockaddr
# include <vector> // std::vector
# include <cstring>// memset
# include <vector> // vector
# include <cstdlib> // atoi
# include "../includes/User.hpp" // User Class
# include <map> // std::map

class Server {

	private:

		int _socketFD;
		int _port;
		std::vector<pollfd> _pollFds;
		std::map<unsigned int, User *> _users;
		unsigned int	_id;
		
	public:

		Server();
		Server( int, int & );
		~Server();
		User	*newUserConnexion( size_t i, int client_fd );
		void    runServer();
};
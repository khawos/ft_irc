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
# include "../includes/User.hpp" // User Class - std::string
# include <map> // std::map

class Server {

	private:

		int								_socketFD;
		int								_port;
		std::vector<pollfd>				_pollFds;
		std::map<unsigned int, User *>	_users;
		unsigned int					_id;
		std::string						_password;
		
	public:

		Server();
		Server( int, int &, std::string );
		~Server();
		User	*newUserConnexion( size_t i, int client_fd );
		void    runServer();
		void	handleCommand( std::string, std::vector<std::string>, int );
		void	handleConnexion( std::vector<std::string> args, int id );
		void	disconnectClient( int id );
		void	handleUserData(size_t pollIndex);
		void	processCommand(User& client, const std::string& line);
};
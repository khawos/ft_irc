#pragma once
# include <sys/socket.h> // socket() listen() send()
# include <iostream> // cout cerr
# include <unistd.h> // close()
# include <fcntl.h> // fcntl()
# include <poll.h> // poll() - struct pollfd
# include <netinet/in.h> // struct sockaddr
# include <vector> // std::vector
# include <cstring>// memset
# include <cstdlib> // atoi
# include <map> // std::map
# include "../includes/User.hpp" // User Class - std::string
# include "../includes/Channel.hpp" // Channel Class 
# include "../includes/Command.hpp" // Command struct - std::string, std::vector<std::string>

class Server {

	private:

		int										_socketFD;
		int										_port;
		std::vector<pollfd>						_pollFds;
		std::map<int, User *>					_users;	// clé = fd du client (PAS l'index dans _pollFds)
		std::map<std::string, Channel *>		_channels;
		std::string								_password;	
		std::string								_serverName;
	
	public:

		Server();
		Server( int, int &, std::string );
		~Server();
		User	*newUserConnexion( int client_fd );
		void	runServer();
		void	handleCommand( std::string, std::vector<std::string>, int );
		void	handleConnexion( std::vector<std::string> args, int fd );
		void	disconnectClient( size_t pollIndex );
		void	handleUserData(size_t pollIndex);
		void	processCommand(User& client, const std::string& line);

		void	handlePass(User &client, Command cmd);
		void	handleNick(User &client, Command cmd);
		void	handleJoin(User &client, Command cmd);
		void	handleUsername(User &client, Command cmd);
		void    handlemode(User &client, Command cmd);
		void    handletopic(User &client, Command cmd);
		void	handlePrivmsg(User &client, Command cmd);
		void	handleKick(User &client, Command cmd);
		void	handleInvite(User &client, Command cmd);

		void	sendWelcomeMessages( User &client );

		bool	isUser( std::string userNickName );
		User	*getUser( std::string nickname ) const;
};

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Client.hpp"
#include "Command.hpp"

class Server
{
	private:

		int						_socketFD;		// socket d ecoute
		int						_port;
		std::string				_password;
		std::vector<pollfd>		_pollFds;		// tableau de pollfd pour poll()
		std::map<int, Client*>	_clients;		// fd -> Client
		bool					_running;		// pour arrete prorement (ctrl+c)

		// Network setup
		void	setNonBlocking(int fd);
		void	initSocket();

		// Poll loop helpers
		void	handleNewConnection();
		void	handleClientData(size_t pollIndex);
		void	disconnectClient(size_t pollIndex);

		// Command processing
		void	processCommand(Client& client, const std::string& line);
		void	tryRegister(Client& client);

		// Command handlers (defined in srcs/commands/*.cpp)
		void	handlePass(Client& client, const Command& cmd);
		void	handleNick(Client& client, const Command& cmd);
		void	handleUser(Client& client, const Command& cmd);
		void	handlePing(Client& client, const Command& cmd);
		void	handleQuit(Client& client, const Command& cmd);

		// Utility: find a client by nickname (returns NULL if not found)
		Client*	findClientByNick(const std::string& nick);

	public:

		Server(int port, const std::string& password);
		~Server();

		void	runServer();
		void	stop();

		// Public utility used by command handlers
		void	sendToClient(Client& client, const std::string& msg);
};

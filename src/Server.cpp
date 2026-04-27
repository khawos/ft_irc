#include "Server.hpp"
#include <csignal>

// Global pointer for signal handler (only way in C++98 to access from a static)
static Server*	g_server = NULL;

static void	signalHandler(int sig)
{
	(void)sig;
	if (g_server)
		g_server->stop();
}

Server::Server(int port, const std::string& password)
	: _socketFD(-1), _port(port), _password(password), _running(false)
{
	g_server = this;
	std::signal(SIGINT, signalHandler);
	std::signal(SIGQUIT, signalHandler);
}

Server::~Server()
{
	// Close all client sockets
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();

	if (_socketFD != -1)
		close(_socketFD);

	g_server = NULL;
}

void	Server::stop()
{
	_running = false;
	std::cout << std::endl << "[Server] Shutting down..." << std::endl;
}

void	Server::setNonBlocking(int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

void	Server::initSocket()
{
	struct sockaddr_in	addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;						// Ipv4
	addr.sin_port = htons(_port);					// htons convertit un entier court de l'ordre d'octets de l'hôte à l'ordre d'octets du réseau (big-endian)
	addr.sin_addr.s_addr = INADDR_ANY;				// toutes les interfaces (0.0.0.0)

	_socketFD = socket(AF_INET, SOCK_STREAM, 0);	// donc serveur pour ipv4, SOCK_STREAM pour TCP, 0 pour le protocole par défaut (TCP dans ce cas)
	if (_socketFD == -1)
		throw std::runtime_error("socket() failed");

	int	opt = 1;
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed");

	setNonBlocking(_socketFD);

	if (bind(_socketFD, (const sockaddr*)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("bind() failed");

	if (listen(_socketFD, 1024) == -1)
		throw std::runtime_error("listen() failed");

	pollfd	serverPoll;
	serverPoll.fd = _socketFD;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	_pollFds.push_back(serverPoll);

	std::cout << "[Server] Listening on port " << _port << std::endl;
}

void	Server::runServer()
{
	initSocket();
	_running = true;

	while (_running)
	{
		int ret = poll(_pollFds.data(), _pollFds.size(), -1);
		if (ret == -1)
		{
			if (!_running)
				break;
			std::cerr << "[Server] poll() error" << std::endl;
			continue;
		}

		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _socketFD)
					handleNewConnection();
				else
				{
					handleClientData(i);
					// handleClientData might have erased _pollFds[i], adjust:
					if (i < _pollFds.size() && _pollFds[i].fd == -1)
					{
						_pollFds.erase(_pollFds.begin() + i);
						i--;
					}
				}
			}
			else if (_pollFds[i].revents & (POLLHUP | POLLERR))
			{
				if (_pollFds[i].fd != _socketFD)
				{
					disconnectClient(i);
					i--;
				}
			}
		}
	}
}

void	Server::handleNewConnection()
{
	int	clientFd = accept(_socketFD, NULL, NULL);
	if (clientFd == -1)
	{
		std::cerr << "[Server] accept() failed" << std::endl;
		return;
	}

	setNonBlocking(clientFd);

	pollfd	clientPoll;
	clientPoll.fd = clientFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;
	_pollFds.push_back(clientPoll);

	_clients[clientFd] = new Client(clientFd);

	std::cout << "[Server] New client connected (fd=" << clientFd << ")" << std::endl;
}

void	Server::handleClientData(size_t pollIndex)
{
	int		fd = _pollFds[pollIndex].fd;
	char	buffer[1024];

	int	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		disconnectClient(pollIndex);
		return;
	}

	buffer[bytes] = '\0';
	Client*	client = _clients[fd];
	client->appendToReadBuffer(std::string(buffer, bytes));

	std::string	line;
	while (client->extractCommand(line))
	{
		if (line.empty())
			continue;
		std::cout << "[RECV fd=" << fd << "] " << line << std::endl;
		processCommand(*client, line);
	}
}

void	Server::disconnectClient(size_t pollIndex)
{
	int	fd = _pollFds[pollIndex].fd;
	std::cout << "[Server] Client disconnected (fd=" << fd << ")" << std::endl;

	close(fd);
	if (_clients.count(fd))
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
	_pollFds.erase(_pollFds.begin() + pollIndex);
}

void	Server::sendToClient(Client& client, const std::string& msg)
{
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

Client*	Server::findClientByNick(const std::string& nick)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return it->second;
	}
	return NULL;
}

void	Server::processCommand(Client& client, const std::string& line)
{
	Command	cmd = parseCommand(line);
	if (cmd.name.empty())
		return;

	// Debug print
	std::cout << "[PARSED] name=" << cmd.name << " params=";
	for (size_t i = 0; i < cmd.params.size(); i++)
		std::cout << "[" << cmd.params[i] << "] ";
	std::cout << std::endl;

	// Dispatcher
	if (cmd.name == "PASS")
		handlePass(client, cmd);
	else if (cmd.name == "NICK")
		handleNick(client, cmd);
	else if (cmd.name == "USER")
		handleUser(client, cmd);
	else if (cmd.name == "PING")
		handlePing(client, cmd);
	else if (cmd.name == "QUIT")
		handleQuit(client, cmd);
	else if (cmd.name == "CAP")
	{
		// Minimal CAP handling for clients like HexChat
		if (!cmd.params.empty() && cmd.params[0] == "LS")
			sendToClient(client, ":server CAP * LS :\r\n");
		// LIST / REQ / END: silently ignored
	}
	else
	{
		// Not registered yet — many commands are blocked
		if (!client.isRegistered())
		{
			std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
			sendToClient(client, ":server 451 " + nick + " :You have not registered\r\n");
		}
		else
		{
			sendToClient(client, ":server 421 " + client.getNickname() + " " + cmd.name + " :Unknown command\r\n");
		}
	}
}

void	Server::tryRegister(Client& client)
{
	if (client.isRegistered())
		return;
	if (!client.isPassOk() || !client.isNickOk() || !client.isUserOk())
		return;

	client.setRegistered(true);
	const std::string&	nick = client.getNickname();

	sendToClient(client, ":server 001 " + nick + " :Welcome to the IRC Network " + nick + "\r\n");
	sendToClient(client, ":server 002 " + nick + " :Your host is server, running version 1.0\r\n");
	sendToClient(client, ":server 003 " + nick + " :This server was created today\r\n");
	sendToClient(client, ":server 004 " + nick + " server 1.0 o o\r\n");

	std::cout << "[Server] Client " << nick << " registered (fd=" << client.getFd() << ")" << std::endl;
}

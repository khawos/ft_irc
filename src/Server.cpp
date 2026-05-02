#include "../includes/Server.hpp"

Server::Server( int port, int &flag, std::string password ) : _socketFD(-1), _port(port), _password(password)
{
	struct	sockaddr_in	socketAddr;
	std::memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(_port); 		// htons is for big endian standard imposed by TCP/IP
	socketAddr.sin_addr.s_addr = INADDR_ANY;

	_socketFD = socket(AF_INET, SOCK_STREAM, 0); // create a socket
	if (_socketFD == -1)
	{
		std::cerr << "(Server Socket) Socket initialization failed." << std::endl;
		flag = -1;
		return ;
	}

	// SO_REUSEADDR pour pouvoir relancer le serveur sans attendre
	int opt = 1;
	setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Mettre le socket d'écoute en non-bloquant (sinon accept() peut bloquer)
	fcntl(_socketFD, F_SETFL, O_NONBLOCK);

	int	bind_return_value = bind( _socketFD, ( const sockaddr *) &socketAddr, sizeof(socketAddr));
	if ( bind_return_value == -1 )
	{
		std::cerr << "(Server Socket) Bind failed to set port " << _port << " as active listening" << std::endl;
		flag = -1;
		return ;
	}

	int	listen_return_value = listen( _socketFD, 1024 ); // backlog = 1024 (PAS le port !)
	if (listen_return_value == -1)
	{
		std::cerr << "(Server Socket) Listen failed to mark _socketFD." << std::endl;
		flag = -1;
		return ;
	}

	pollfd serverPoll;
	serverPoll.fd = _socketFD;
	serverPoll.events = POLLIN;
	_pollFds.push_back(serverPoll);
	return ;
}

Server::~Server()
{
	// Fermer tous les fds clients et delete les User*
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	_users.clear();

	// Puis fermer le socket d'écoute
	if (_socketFD != -1)
		close(_socketFD);
}

void	Server::runServer()
{
	bool			running = true;
	while ( running )
	{
		poll( _pollFds.data(), _pollFds.size(), -1 );
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if ( _pollFds[i].revents & POLLIN )
			{
				if (_pollFds[i].fd == _socketFD)
				{
					int client_fd = accept(_socketFD, NULL, NULL);
					if (client_fd != -1)
					{
						// On stocke par fd, pas par index !
						_users[client_fd] = newUserConnexion(client_fd);
					}
					else
						std::cerr << "Accept failed" << std::endl;
				}
				else
				{
					handleUserData( i );
					// Si le client a été déconnecté, l'index a changé
					if (i < _pollFds.size() && _pollFds[i].fd == -1)
					{
						_pollFds.erase(_pollFds.begin() + i);
						i--;
					}
				}
			}
		}
	}
}

// On prend l'index dans _pollFds pour savoir quoi supprimer
// (l'index suffit puisqu'on a accès à _pollFds[id].fd pour retrouver le User)
void	Server::disconnectClient( size_t pollIndex )
{
	int fd = _pollFds[pollIndex].fd;
	std::cout << "Client (fd=" << fd << ") disconnected from the server" << std::endl;

	// Fermer le socket
	close(fd);

	// Delete le User et l'enlever du map (par fd, pas par index)
	if (_users.count(fd))
	{
		delete _users[fd];
		_users.erase(fd);
	}

	// Enlever du _pollFds
	_pollFds.erase(_pollFds.begin() + pollIndex);
}

User	*Server::newUserConnexion( int client_fd )
{
	std::cout << "New client connected! (fd=" << client_fd << ")" << std::endl;

	// fcntl correct : fcntl(fd, F_SETFL, O_NONBLOCK)
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd clientPoll;
	clientPoll.fd = client_fd;
	clientPoll.events = POLLIN;
	_pollFds.push_back(clientPoll);

	User *newUser = new User(client_fd);  // on passe le fd comme id et fd
	std::cout << "Client (fd=" << client_fd << ") has been added to the server" << std::endl;
	return newUser;
}

void	Server::handleUserData(size_t pollIndex)
{
	int		fd = _pollFds[pollIndex].fd;
	char	buffer[1024];

	int	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

	// CHECK AVANT de toucher buffer[bytes] !
	if (bytes <= 0)
	{
		disconnectClient(pollIndex);
		return;
	}
	buffer[bytes] = '\0';

	// Récupérer le User par son fd (clé stable)
	if (!_users.count(fd))
	{
		std::cerr << "No user found for fd " << fd << std::endl;
		return;
	}
	User *user = _users[fd];

	user->appendToReadBuffer(std::string(buffer, bytes));
	std::string	line;
	while (user->extractCommand(line))
	{
		if (line.empty())
			continue;
		std::cout << "[RECV fd=" << fd << "] " << line << std::endl;
		processCommand( *user, line );
	}
}

void	Server::processCommand(User& client, const std::string& line)
{
	Command	cmd = parseCommand(line);
	if (cmd.name.empty())
		return;

	std::cout << "[PARSED] name=" << cmd.name << " params=";
	for (size_t i = 0; i < cmd.params.size(); i++)
		std::cout << "[" << cmd.params[i] << "] ";
	std::cout << std::endl;

	// Dispatcher
	if (cmd.name == "PASS")
		handlePass(client, cmd);
	else if ( client.isPassOk() != true )
	{
		send(client.getFd(), "Enter the password : PASS <password>\n", 38, 0);
		return ;
	}
 	else if (cmd.name == "NICK")
 		handleNick(client, cmd);
	else if (cmd.name == "USER")
		handleUsername(client, cmd);
	else if (cmd.name == "JOIN")
		handleJoin(client, cmd);
	else if (cmd.name == "MODE")
		handlemode(client, cmd);
// 	else if (cmd.name == "QUIT")
// 		handleQuit(client, cmd);
// 	else if (cmd.name == "CAP")
// 	{
// 		// Minimal CAP handling for clients like HexChat
// 		if (!cmd.params.empty() && cmd.params[0] == "LS")
// 			sendToClient(client, ":server CAP * LS :\r\n");
// 		// LIST / REQ / END: silently ignored
// 	}
// 	else
// 	{
// 		// Not registered yet — many commands are blocked
// 		if (!client.isRegistered())
// 		{
// 			std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
// 			sendToClient(client, ":server 451 " + nick + " :You have not registered\r\n");
// 		}
// 		else
// 		{
// 			sendToClient(client, ":server 421 " + client.getNickname() + " " + cmd.name + " :Unknown command\r\n");
// 		}
// 	}
}

#include "../includes/Server.hpp"

Server::Server( int port, int &flag, std::string password ) : _id(0), _password(password)
{
	_port = port; //to change latter and put in the constructor
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
	int	bind_return_value = bind( _socketFD, ( const sockaddr *) &socketAddr, sizeof(socketAddr)); // bind the socket to the port
	if ( bind_return_value == -1 )
	{
		std::cerr << "(Server Socket) Bind failed to set port " << _port << " as active listening" << std::endl;
		flag = -1;
		return ;
	}
	int	listen_return_value = listen( _socketFD, port ); // start leastening for incoming connections
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
	close( _socketFD );
	for ( std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); it++ )
		close( it->fd );
	for ( size_t i = _users.size(); i != 0; i-- )
		delete _users[i];
}



void	Server::runServer()
{
	bool	running = true;
	while ( running )
	{
		poll( _pollFds.data(), _pollFds.size(), -1 );				// check all the socket at the same time
		for (size_t i = 0; i < _pollFds.size(); i++)				// on boucle sur tous les sockets
		{
			if ( _pollFds[i].revents & POLLIN )						// on regarde si il y a de la data a lire
			{
				if (_pollFds[i].fd == _socketFD)					// est ce que c'est le socket pour se connecter au serveur ?
				{
					int client_fd = accept(_socketFD, NULL, NULL);	// ouvre un socket pour la connection entrente
					if (client_fd != -1)
						_users[ i ] = newUserConnexion( i, client_fd );
					else
						std::cerr << "Accept failed" << std::endl;
				}
				else
				{
					handleUserData( i );
					// char	buffer[1024];
					// int		bytes = recv(_pollFds[i].fd, buffer, 1023, 0); // catch data form the socket
					// if (bytes <= 0)
					// {
					// 	close(_pollFds[i].fd);
					// 	_pollFds.erase(_pollFds.begin() + i); // remove the socket
					// 	i--;
					// }
					// else
					// {
					// 	buffer[bytes] = '\0';
					// 	std::cout << buffer << std::endl;
					// }
				}
			}
		}
	}	
}

void	Server::disconnectClient( int id )
{
	close( _pollFds[id].fd );
	_pollFds.erase( _pollFds.begin() + id);
	delete _users[id - 1];
}

void	Server::handleConnexion( std::vector<std::string> args, int id )
{
	if ( args[0] != _password )
	{
		disconnectClient( id );
		std::cout << "Wrong password try again" << std::endl;
	}
	else
		std::cout << "Successfuly connected" << std::endl;
}

void	Server::handleCommand( std::string cmd, std::vector<std::string> args, int id )
{
	std::cout << " cmd : " << cmd << std::endl;
	if ( cmd == "PASS" )
	{
		handleConnexion( args, id );
	}
}

User	*Server::newUserConnexion( size_t i, int client_fd )
{
	std::cout << "New client connected!" << std::endl;
	User *newUser = new User( i );
	fcntl( client_fd, O_NONBLOCK );
	pollfd clientPoll;
	clientPoll.fd = client_fd;
	clientPoll.events = POLLIN;
	_pollFds.push_back( clientPoll );
	std::cout << "Client number " << i << " has been added to the server" << std::endl;
	return ( newUser );
}

void	Server::handleUserData(size_t pollIndex)
{
	int		fd = _pollFds[pollIndex].fd;
	char	buffer[1024];

	int	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	buffer[bytes] = '\0';
	if (bytes <= 0)
	{
		std::cout << "Client number " << pollIndex - 1 << " has been disconected form the server" << std::endl;
		disconnectClient(pollIndex);
		return;
	}

	User *user = _users[pollIndex - 1];
	if (!user)
	{
		std::cerr << "No user found for fd " << pollIndex << std::endl;
		return;
	}
	user->appendToReadBuffer(std::string(buffer, bytes));
	std::string	line;
	while (user->extractCommand(line))
	{
		if (line.empty())
			continue;
		std::cout << "[RECV fd=" << fd << "] " << line << std::endl;
		processCommand(*client, line);
	}
}

#include "Server.hpp"

Server::Server(){}
Server::~Server()
{
	close(_socketFD);
}
void	Server::runServer()
{
	bool	running = true;
	int		poll_return_value;		
	struct	sockaddr_in	socketAddr;

	std::memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(_port); // hrons is for big endian standard imposed by TCP/IP
	socketAddr.sin_addr.s_addr = INADDR_ANY;

	_socketFD = socket(AF_INET, SOCK_STREAM, 0); // create a socket
	if (_socketFD == -1)
	{
		std::cerr << "(Server Socket) Socket initialization failed." << std::endl;
		return ;
	}

	int	bind_return_value = bind( _socketFD, ( const sockaddr *) &socketAddr, sizeof(socketAddr)); // bind the socket to the port
	if ( bind_return_value == -1 )
	{
		std::cerr << "(Server Socket) Bind failed to set port " << _port << " as active listening" << std::endl;
		return ;
	}

	int	listen_return_value = listen( _socketFD, 1024 ); // start leastening for incoming connections
	if (listen_return_value == -1)
	{
		std::cerr << "(Server Socket) Listen failed to mark _socketFD." << std::endl;
		return ;
	}
	
	while ( running )
	{
	}	
}
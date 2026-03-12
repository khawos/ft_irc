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
	
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = _port;
	socketAddr.sin_addr.s_addr = INADDR_ANY;

	_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFD == -1)
	{
		std::cerr << "(Server Socket) Socket initialization failed." << std::endl;
		return ;
	}

	int	listen_return_value = listen( _socketFD, 1024 );
	if (listen_return_value == -1)
	{
		std::cerr << "(Server Socket) Listen failed to mark _socketFD." << std::endl;
		return ;
	}
	
	int	bind_return_value = bind( _socketFD, ( const sockaddr *) &socketAddr, sizeof(socketAddr));
	if ( bind_return_value == -1 )
	{
		std::cerr << "(Server Socket) Bind failed to set port " << _port << " as active listening" << std::endl;
		return ;
	}

	while ( running )
	{
	}	
}
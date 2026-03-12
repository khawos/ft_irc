#include "Server.hpp"
#include <string.h>

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

	memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(_port); // htons is for big endian standard imposed by TCP/IP
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
	pollfd serverPoll;
	serverPoll.fd = _socketFD;
	serverPoll.events = POLLIN;
	_pollFds.push_back(serverPoll);
	
	while (running)
	{
		poll(_pollFds.data(), _pollFds.size(), -1);	// check all the socket at the same time
	
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _socketFD)
				{
					int client_fd = accept(_socketFD, NULL, NULL);
					if (client_fd != -1)
					{
						std::cout << "New client connected!" << std::endl;
						pollfd clientPoll;
						clientPoll.fd = client_fd;
						clientPoll.events = POLLIN;
						_pollFds.push_back(clientPoll);
					}
					else
						std::cerr << "Accept faild" << std::endl;
				}
				else
				{
					char	buffer[1024];
					int		bytes = recv(_pollFds[i].fd, buffer, 1023, 0); // catch data form the socket

					if (bytes <= 0)
					{
						close(_pollFds[i].fd);
						_pollFds.erase(_pollFds.begin() + i); // remove the socket
						i--;
					}
					else
					{
						buffer[bytes] = '\0';
						std::cout << buffer << std::endl;
					}
				}
			}
		}
	}	
}
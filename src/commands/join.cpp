#include "../../includes/Server.hpp"

void    Server::handleJoin(User &client, Command cmd)
{
	try
	{
		_channels.at( cmd.params[1] );
		Channel *channel = _channels[ cmd.params[1] ];
		channel->addUserInChannel( client );
	}
	catch(const std::exception& e)
	{
		std::string pass = cmd.params[2];
		std::string name = cmd.params[1];
		int			fd = socket(AF_INET, SOCK_STREAM, 0);
		pollfd		pfd;

		pfd.fd = fd;
		pfd.events = POLLIN;
		_pollFds.push_back(pfd);
		Channel *channel = new Channel( fd, client.getUsername(), pass, name);
		_channels[ name ] = channel;
	}	
}
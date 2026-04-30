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
		Channel *channel = new Channel( client.getUsername(), pass, name );
		channel->addUserInChannel( client );
		_channels[ name ] = channel;
	}	
}
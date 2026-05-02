#include "../../includes/Server.hpp"

void Server::handleNick(User &client, Command cmd)
{
	if ( cmd.params.size() != 1 )
	{
		send(client.getFd(), "Use NICK <nickname>\n", 21, 0);
	}
	else if ( client.isNickOk() != true )
	{
		client.setNickname(cmd.params[0]);
		client.setNickOk( true );
		if (client.isUserOk())
		{
			client.setRegistered(true);   
			sendWelcomeMessages( client );
		}
	}
}
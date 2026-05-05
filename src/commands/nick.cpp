#include "../../includes/Server.hpp"

/**
 * @author			Jbayeonne
 * 
 * @param	client	calling client
 * @param	cmd		struct of params
 * 
 * @note			Allow client to set their nick after enter the password
 */
void Server::handleNick(User &client, Command cmd)
{
	if ( cmd.params.size() != 1 )
	{
		send(client.getFd(), "Use NICK <nickname>\n", 21, 0);
	}
	else
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
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
	else if ( nickIsAlreadyTaken( cmd.params[0] ) )
	{
		std::string msg = ":" + _serverName + " 433 * " + cmd.params[0] + " :Nickname is already in use.\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
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
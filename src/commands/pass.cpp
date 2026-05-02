#include "../../includes/Server.hpp"

/**
 * @author			Jbayeonne
 * 
 * @param	client	calling client
 * @param	cmd		struct of params
 * 
 * @note			Allow client to connect to the server
 * @note			First command to enter as a client
 */
void    Server::handlePass(User &client, Command cmd)
{
	if (cmd.params.size() != 1)
	{
		send(client.getFd(), "Use : PASS <password>\n", 23, 0);
	}
	else if ( cmd.params[0] != _password )
	{
		send(client.getFd(), "Wrong password try again\n", 26, 0);
	}
	else if ( cmd.params[0] == _password )
	{
		client.setPassOk( true );
		send(client.getFd(), "Succesfully Connected\n", 23, 0);
	}
}

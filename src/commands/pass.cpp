#include "../../includes/Server.hpp"

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
		send(client.getFd(), "Use 'NICK <your_nickname>' in order to initialize your nickname\n", 65, 0);
		send(client.getFd(), "Use 'USER <your_nickname>' in order to initialize your username\n", 65, 0);
	}
}

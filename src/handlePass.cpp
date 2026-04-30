#include "../includes/Server.hpp"

void    Server::handlePass(User &client, Command cmd)
{
	if (cmd.params.size() != 1)
	{
		send(_pollFds[client.getId() + 1].fd, "Use : PASS <password>\n", 23, 0);
	}
	else if ( cmd.params[0] != _password )
	{
		send(_pollFds[client.getId() + 1].fd, "Wrong password try again\n", 26, 0);
	}
	else if ( cmd.params[0] == _password )
	{
		client.setPassOk( true );
		send(_pollFds[client.getId() + 1].fd, "Succesfully Connected\n", 23, 0);
	}
}

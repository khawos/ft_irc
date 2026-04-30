#include "../includes/Server.hpp"

void    Server::handlePass(User &client, Command cmd)
{
	std::cout << "SERVER: Handling PASS command for fd=" << client.getFd() << std::endl;
	if (cmd.params.size() != 1)
	{
		send(_pollFds[client.getFd()].fd, "Use : PASS <password>\n", 23, 0);
	}
	else if ( cmd.params[0] != _password )
	{
		send(_pollFds[client.getFd()].fd, "Wrong password try again\n", 26, 0);
	}
	else if ( cmd.params[0] == _password )
	{
		client.setPassOk( true );
		send(_pollFds[client.getFd()].fd, "Succesfully Connected\n", 23, 0);
	}
}

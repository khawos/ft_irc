#include "Server.hpp"

void	Server::handlePass(Client& client, const Command& cmd)
{
	if (client.isRegistered())
	{
		sendToClient(client, ":server 462 " + client.getNickname() + " :You may not reregister\r\n");
		return;
	}
	if (cmd.params.empty())
	{
		sendToClient(client, ":server 461 * PASS :Not enough parameters\r\n");
		return;
	}
	if (cmd.params[0] != _password)
	{
		sendToClient(client, ":server 464 * :Password incorrect\r\n");
		return;
	}
	client.setPassOk(true);
}

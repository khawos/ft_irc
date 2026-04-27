#include "Server.hpp"

void	Server::handleUser(Client& client, const Command& cmd)
{
	if (client.isRegistered())
	{
		sendToClient(client, ":server 462 " + client.getNickname() + " :You may not reregister\r\n");
		return;
	}
	if (cmd.params.size() < 4)
	{
		std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
		sendToClient(client, ":server 461 " + nick + " USER :Not enough parameters\r\n");
		return;
	}

	client.setUsername(cmd.params[0]);
	client.setRealname(cmd.params[3]);
	client.setUserOk(true);

	tryRegister(client);
}

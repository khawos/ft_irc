#include "Server.hpp"

void	Server::handlePing(Client& client, const Command& cmd)
{
	std::string	token = cmd.params.empty() ? "" : cmd.params[0];
	sendToClient(client, ":server PONG server :" + token + "\r\n");
}

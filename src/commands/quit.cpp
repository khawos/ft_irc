#include "Server.hpp"

void	Server::handleQuit(Client& client, const Command& cmd)
{
	std::string	reason = cmd.params.empty() ? "Client quit" : cmd.params[0];
	std::string	msg = "ERROR :Closing link: (" + client.getNickname() + ") [" + reason + "]\r\n";
	sendToClient(client, msg);

	// The actual disconnection will happen on the next poll iteration
	// when recv() returns 0 after the client closes its side, OR we can
	// trigger it by shutting the socket down here. For simplicity we let
	// the client close, but if you want to force it, do:
	//   shutdown(client.getFd(), SHUT_RDWR);
	// The poll loop will then detect POLLHUP and call disconnectClient.
	(void)client;
}

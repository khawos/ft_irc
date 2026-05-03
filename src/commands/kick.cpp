#include "../../includes/Server.hpp"

void	Server::handleKick(User &client, Command cmd)
{
	if (cmd.params.size() < 2)
	{
		std::string err = ":" + _serverName + " 461 " + client.getNickname() + " KICK :Not enough parameters\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	std::string channelName = cmd.params[0];
	std::string targetNick  = cmd.params[1];
	std::string reason      = (cmd.params.size() >= 3) ? cmd.params[2] : "No reason";

	if (_channels.find(channelName) == _channels.end()) // channel n'existe pas
	{
		std::string err = ":" + _serverName + " 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	Channel *channel = _channels[channelName];

	if (!channel->isOperator(client)) // client n'est pas opérateur du channel
	{
		std::string err = ":" + _serverName + " 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	User *target = getUser(targetNick);
	if (!target)	// target n'existe pas
	{
		std::string err = ":" + _serverName + " 401 " + client.getNickname() + " " + targetNick + " :No such nick\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	const std::map<std::string, User> &members = channel->getUserMap();
	if (members.find(target->getUsername()) == members.end()) // target n'est pas dans le channel
	{
		std::string err = ":" + _serverName + " 441 " + client.getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	std::string kickMsg = ":" + client.getNickname() + "!" + client.getUsername()
		+ "@127.0.0.1 KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
	channel->sendMessage_broadcast(kickMsg); // Informer tous les membres du channel du kick

	channel->deleteUserFromChannel(*target); // Enlever le user du channel
	channel->decNbUSer();
}

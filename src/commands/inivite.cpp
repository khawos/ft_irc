#include "../../includes/Server.hpp"

void	Server::handleInvite(User &client, Command cmd)
{
	if (cmd.params.size() < 2)
	{
		std::string err = ":" + _serverName + " 461 " + client.getNickname() + " INVITE :Not enough parameters\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	std::string targetNick  = cmd.params[0];
	std::string channelName = cmd.params[1];

	if (_channels.find(channelName) == _channels.end()) // channel n'existe pas
	{
		std::string err = ":" + _serverName + " 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	Channel *channel = _channels[channelName];

	if (channel->getInviteMode() && !channel->isOperator(client)) // client n'est pas opérateur du channel et le channel est en mode invite
	{
		std::string err = ":" + _serverName + " 482 " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	User *target = getUser(targetNick);
	if (!target)  // target n'existe pas
	{
		std::string err = ":" + _serverName + " 401 " + client.getNickname() + " " + targetNick + " :No such nick\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	const std::map<std::string, User> &members = channel->getUserMap();
	if (members.find(target->getUsername()) != members.end()) // target est déjà dans le channel
	{
		std::string err = ":" + _serverName + " 443 " + client.getNickname() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	channel->addToInviteList(*target); // Ajouter le target à la liste d'invitation du channel

	std::string reply = ":" + _serverName + " 341 " + client.getNickname() + " " + targetNick + " " + channelName + "\r\n";
	send(client.getFd(), reply.c_str(), reply.size(), 0);  // Confirmation à l'inviteur

	std::string inviteMsg = ":" + client.getNickname() + "!" + client.getUsername()
		+ "@127.0.0.1 INVITE " + targetNick + " :" + channelName + "\r\n";
	send(target->getFd(), inviteMsg.c_str(), inviteMsg.size(), 0); // Informer le target de l'invitation
}

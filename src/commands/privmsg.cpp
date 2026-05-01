#include "../../includes/Server.hpp"

void	Server::handlePrivmsg(User &client, Command cmd)
{
	if (!client.isRegistered())
	{
		std::string err = ":server 451 * :You have not registered\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}
	if (cmd.params.size() < 2)
	{
		std::string err = ":server 461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(client.getFd(), err.c_str(), err.size(), 0);
		return;
	}

	std::string target = cmd.params[0];
	std::string message = cmd.params[1];
	std::string fullMsg = ":" + client.getNickname() + "." + client.getUsername()
		+ "PRIVMSG " + target + " :" + message + "\r\n";

	if (target[0] == '#') // emvoier à tous les membres du channel sauf le sender
	{
		if (_channels.find(target) == _channels.end()) // chercher le channel
		{
			std::string err = ":server 403 " + client.getNickname() + " " + target + " :No such channel\r\n";
			send(client.getFd(), err.c_str(), err.size(), 0);
			return;
		}
		const std::map<std::string, User> &members = _channels[target]->getUserMap();				// récupérer les membres du channel
		for (std::map<std::string, User>::const_iterator it = members.begin(); it != members.end(); ++it) // envoyer le message à tous les membres sauf le sender
		{																									// peut pototiellement etre remplace par la fonction sendMessage() mais ATM ca marche.
			if (it->second.getFd() != client.getFd())
				send(it->second.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
		}
	}
	else
	{
		User *dest = NULL;
		for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			if (it->second->getNickname() == target)
			{
				dest = it->second;
				break;
			}
		}
		if (!dest)
		{
			std::string err = ":server 401 " + client.getNickname() + " " + target + " :No such nick\r\n";
			send(client.getFd(), err.c_str(), err.size(), 0);
			return;
		}
		send(dest->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
	}
}

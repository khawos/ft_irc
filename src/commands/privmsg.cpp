#include "../../includes/Server.hpp"

/**
 * 
 * @author	Amedenec
 * 
 * @param	client	calling client
 * @param	cmd		struct of params
 * 
 * @note			Allow client to send message in channel or to other client
 */
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
	std::string fullMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIp() +" PRIVMSG " + target + " :" + message + "\r\n";	
	if (target[0] == '#')
	{
		std::cout << "Message : " << fullMsg << " Channel : " << target << std::endl;
		if ( _channels.find(target) == _channels.end() )
		{
			std::string err = ":server 403 " + client.getNickname() + " " + target + " :No such channel\r\n";
			send(client.getFd(), err.c_str(), err.size(), 0);
			return;
		}
		Channel	*channel = _channels[ target ];
		const std::map<std::string, User> &members = channel->getUserMap();
		if (members.find(client.getUsername()) == members.end())	// client n'est pas dans le channel
		{
			std::string err = ":" + _serverName + " 442 " + client.getNickname() + " " + target + " :You're not on that channel\r\n";
			send(client.getFd(), err.c_str(), err.size(), 0);
			return;
		}
		channel->sendMessage( fullMsg, client );
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

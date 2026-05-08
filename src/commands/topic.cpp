#include "../../includes/Server.hpp"

/**
 * 
 * @author		Jbayonne
 * @param		client calling client
 * @param		cmd		struct of params
 * 
 * @note		Allow client to set the topic of a channel
 */
void    Server::handletopic( User &client, Command cmd )
{
	if ( cmd.params.size() != 2 && cmd.params.size() != 1 )
	{
		send( client.getFd(), "Use : TOPIC <channel> [<topic>]\n", 33, 0);
		return ;
	}
	try
	{
		_channels.at( cmd.params[0]) ;
		Channel *channel = _channels[ cmd.params[0] ];
		const std::map<std::string, User> &members = channel->getUserMap();
		if (members.find(client.getUsername()) == members.end())	 // client n'est pas dans le channel
		{
			std::string err = ":" + _serverName + " 442 " + client.getNickname() + " " + channel->getName() + " :You're not on that channel\r\n";
			send(client.getFd(), err.c_str(), err.size(), 0);
			return ;
		}
		if ( channel->getTopic().size() == 0 && cmd.params.size() == 1 )
		{
			std::string reply = ":" + _serverName + " 331 " + client.getNickname() + " " + channel->getName() + " :No topic is set\r\n";
			send(client.getFd(), reply.c_str(), reply.length(), 0);
			return ;
		}
		else if ( channel->getTopic().size() != 0 && cmd.params.size() == 1 )
		{
			std::string reply = ":" + _serverName + " 332 " + client.getNickname() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
			send(client.getFd(), reply.c_str(), reply.length(), 0);
			return ;
		}
		if ( channel->getTopicIsOnlyOp() == true && !channel->isOperator( client ) )
		{
			std::string errMsg = ":" + _serverName + " 482 " + client.getNickname() + " " + channel->getName() + " :You're not channel operator\r\n";
		    send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
			return ;
		}
		channel->setTopic( cmd.params[1] );
		std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getIp();
		std::string broadcastMsg = ":" + prefix + " TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
		channel->sendMessage( broadcastMsg, client );
	}
	catch( std::exception &e )
	{
		send( client.getFd(), "Channel not found\n", 33, 0);
	}
	
}
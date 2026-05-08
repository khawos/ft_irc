#include "../../includes/Server.hpp"

/**
 * 
 * @author 		Jbayonne
 * @param user	joining client
 * 
 * @note		Informe clients when someone join one of their channel 
 * 
 * 
 */
void	Channel::send_relay_message( User &user )
{
	std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getIp() + " JOIN :" + getName() + "\r\n";
	const std::map<std::string, User*>	&_User = getUserMap();
	for ( std::map<std::string, User*>::const_iterator it = _User.begin(); it != _User.end(); it++ )
		send(it->second->getFd(), msg.c_str(), msg.size(), 0);
}

/**
 * 
 * @author 			Jbayonne
 * @param client	calling client
 * @param cmd		struct of params
 * 
 * @note 			Allow client to join a channel or to create a channel
 */
void    Server::handleJoin(User &client, Command cmd)
{
	if ( cmd.params.size() < 1 )
	{
    	std::string errMsg = ":ircserv 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
    	send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
    	return;
	}
	try
	{
		_channels.at( cmd.params[0] );
		Channel *channel = _channels[ cmd.params[0] ];
		/**
		 *  Check for limit -----------------------------------------------------------------------------------------
		 */
		if ( channel->getUserLimit() && channel->getNbUser() >= channel->getUserLimit())
		{
			std::string errMsg = ":ircserv 471 " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+l)\r\n";
    		send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
		    return;
		}
		/**
		 *  Password mode---------------------------------------------------------------------------------------------
		 */
		if ( channel->getKey() == true )
		{
			if ( cmd.params.size() != 2 )
			{
				std::string errMsg = ":ircserv 461 " + client.getNickname() + " JOIN :Not enough parameters : Use JOIN <channel> <password>\r\n";
   				send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
				return;
			}
			else if ( cmd.params[1] != channel->getPassword() )
			{
				std::string errMsg = ":ircserv 475 " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+k)\r\n";
    			send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
				return;
			}
		}
		/**
		 * Invitation mode---------------------------------------------------------------------------------------------
		 */
		if ( channel->getInviteMode() == true && channel->isInvited( client ) == false )
		{
			send( client.getFd(), "This channel is an invite only channel\n", 40, 0);
			return ;
		}
		channel->addUserInChannel( _users[ client.getFd() ] );
		channel->send_relay_message( client );
		channel->incNbUSer();
		sendNamesAfterJoin( client, channel );
	}
	catch( const std::exception& e )
	{
		std::cout << "New channel" << std::endl;
		std::string pass = "";
		if  ( cmd.params.size() > 1 )
			pass = cmd.params[1];
		std::string name = cmd.params[0];
		Channel *channel = new Channel( &client, pass, name );
		_channels[ name ] = channel;
		channel->addUserInChannel( _users[ client.getFd() ] );
		channel->incNbUSer();
		channel->send_relay_message( client );
		sendNamesAfterJoin( client, channel );
	}
}

void	Server::sendNamesAfterJoin( User &client, Channel *channel )
{
	std::string namesList = "";
	const std::map<std::string, User*> &members = channel->getUserMap();
	for (std::map<std::string, User*>::const_iterator it = members.begin(); it != members.end(); it++)
	{
		if (!namesList.empty())
			namesList += " ";
		if (channel->isOperator(*it->second))
			namesList += "@";
		namesList += it->second->getNickname();
	}
	std::string msg332 = ":" + _serverName + " 332 " + client.getNickname() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
	std::string msg353 = ":" + _serverName + " 353 " + client.getNickname() + " = " + channel->getName() + " :" + namesList + "\r\n";
	std::string msg366 = ":" + _serverName + " 366 " + client.getNickname() + " " + channel->getName() + " :End of NAMES list\r\n";
	std::cout << "send name list to " <<  client.getFd() << " " << namesList << std::endl;
	send(client.getFd(), msg332.c_str(), msg332.size(), 0);
	send(client.getFd(), msg353.c_str(), msg353.size(), 0);
	send(client.getFd(), msg366.c_str(), msg366.size(), 0);
}
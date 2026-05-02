#include "../../includes/Server.hpp"

void	Channel::send_relay_message( User &user )
{
	std::string msg = ":" + user.getUsername() + "!127.0.0.1" + "JOIN :" + getName() + "\r\n";
	const std::map<std::string, User>	&_User = getUserMap();
	for ( std::map<std::string, User>::const_iterator it = _User.begin(); it != _User.end(); it++ )
		send(it->second.getFd(), msg.c_str(), msg.size(), 0);
}

void    Server::handleJoin(User &client, Command cmd)
{
	try
	{
		_channels.at( cmd.params[0] );
		Channel *channel = _channels[ cmd.params[0] ];
		/**
		 *  Password mode
		 */
		if ( channel->getKey() == true )
		{
			if ( cmd.params.size() != 2 )
			{
				send( client.getFd(), "Use : JOIN <channel> <password>\n", 33, 0);
				return;
			}
			else if ( cmd.params[1] != channel->getPassword() )
			{
				send( client.getFd(), "Wrong password try again\n", 26, 0);
				return;
			}
		}
		/**
		 * Invitation mode
		 */
		if ( channel->getInviteMode() == true && channel->isInvited( client ) == false )
		{
			send( client.getFd(), "This channel is an invite only channel\n", 40, 0);
			return ;
		}
		channel->addUserInChannel( client );
		channel->send_relay_message( client);
	}
	catch(const std::exception& e)
	{
		std::string pass = "";
		if  ( cmd.params.size()  > 1 )
			pass = cmd.params[1];
		std::string name = cmd.params[0];
		Channel *channel = new Channel( client.getUsername(), pass, name );
		channel->addUserInChannel( client );
		_channels[ name ] = channel;
		channel->send_relay_message( client );
	}	
}
#include "../../includes/Server.hpp"

/**
 * @author 			Jbayonne
 * @param client	Calling client
 * @param cmd		Struct of param
 * 
 * @note			This function allow operator to manage the channel settings
 * @note			-i: change invite mode
 * @note			-k: set / unset password
 * @note			-t: allow / disallow user to change channel's topic 				
 */
void    Server::handlemode(User &client, Command cmd)
{
	if ( cmd.params.size() < 2 )
	{
		return ;
	}
	try
	{
		_channels.at( cmd.params[0] );
		Channel *channel = _channels[ cmd.params[0] ];
		if ( !channel->isOperator( client ) )
		{
			send(client.getFd(), "You must be the channel's operator to operate such action\n", 59, 0);
			return ;
		}
		std::string	option = cmd.params[1];
		// std::cout << option << std::endl; 
		std::string prefix = client.getNickname() + "!" + client.getUsername() + "@" + client.getIp();
		if ( option == "-i" )
		{
			channel->setInviteMode( false );
			std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " -i\r\n";
			channel->sendMessage_broadcast(broadcastMsg);
		}
		else if ( option == "+i")
		{
			channel->setInviteMode( true );
			std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " +i\r\n";
			channel->sendMessage_broadcast(broadcastMsg);
		}
		else if ( option == "-t" )
		{
			channel->setTopicIsOnlyOp( false );
			std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " -t\r\n";
			channel->sendMessage_broadcast(broadcastMsg);
		}
		else if ( option == "+t" )
		{
			channel->setTopicIsOnlyOp( true );
			std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " +t\r\n";
			channel->sendMessage_broadcast(broadcastMsg);
		}
		else if ( option == "-k" || option == "+k" )
		{
			if ( cmd.params.size() != 3 && option == "+k" )
			{
				std::string err = ":" + _serverName + " 461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
				send(client.getFd(), err.c_str(), err.size(), 0);
				return ;
			}
			else if ( cmd.params.size() == 3 && option == "+k" )
			{
				channel->setPassword( cmd.params[2] );
				channel->setKey( true );
				std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " +k " + cmd.params[2] + "\r\n";
				channel->sendMessage_broadcast(broadcastMsg);
			}
			else if ( cmd.params.size() != 2 && option == "-k" )
			{
				std::string err = ":" + _serverName + " 461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
				send(client.getFd(), err.c_str(), err.size(), 0);
				return ;
			}
			else
			{
				channel->setPassword("");
				channel->setKey( false );
				std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " -k\r\n";
				channel->sendMessage_broadcast(broadcastMsg);			
			}
		}
		else if (option == "-o" || option == "+o" )
		{
			if ( cmd.params.size() != 3 )
			{	
				std::string err = ":" + _serverName + " 461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
				send(client.getFd(), err.c_str(), err.size(), 0);
				return ;
			}
			User *user = getUser( cmd.params[2] );
			if ( !user )
			{
				std::string err = ":" + _serverName + " 401 " + client.getNickname() + " " + cmd.params[2] + " :No such nick\r\n";
				send(client.getFd(), err.c_str(), err.size(), 0);
				return ;
			}
			if ( option == "-o" )
				channel->demoteOp( &client, user );
			else
				channel->promoteOp( &client, user );
		}
		else if ( option == "-l" || option == "+l" )
		{
			if ( option == "+l" && cmd.params.size() == 3 )
			{
				channel->setUserLimit( std::atoi( cmd.params[2].c_str() ) );
				std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " +l " + cmd.params[2] + "\r\n";
				channel->sendMessage_broadcast(broadcastMsg);
			}
			else if ( option == "-l" && cmd.params.size() == 2 )
			{
				channel->setUserLimit( 0 );
				std::string broadcastMsg = ":" + prefix + " MODE " + channel->getName() + " -l\r\n";
				channel->sendMessage_broadcast(broadcastMsg);
			}
			else
			{
				std::string err = ":" + _serverName + " 461 " + client.getNickname() + " MODE :Not enough parameters\r\n";
				send(client.getFd(), err.c_str(), err.size(), 0);
				return ;
			}
		}
	}
	catch( const std::exception& e )
	{
		// std::cout << " THROW" << std::endl;
		// send( client.getFd(), "Use : MODE <channel> <flags>\n", 30, 0 );
		// send( client.getFd(), "ERROR:MODE <channel> <flags>\n", 30, 0 );
		return ;
	}

}

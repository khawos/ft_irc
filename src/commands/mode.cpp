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
		send(client.getFd(), "Use : MODE <channel> <flags>\n", 30, 0);
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
		if ( cmd.params[1] == "-i" )
			channel->setInviteMode( false );
		else if ( cmd.params[1] == "+i")
			channel->setInviteMode( true );
		else if ( cmd.params[1] == "-t" )
			channel->setTopicIsOnlyOp( false );
		else if ( cmd.params[1] == "+t" )
			channel->setTopicIsOnlyOp( true );
		else if ( cmd.params[1] == "-k" || cmd.params[1] == "+k" )
		{	
			if ( cmd.params.size() != 3 && cmd.params[1] == "+k" )
			{
				// error
			}
			else if ( cmd.params.size() != 3 && cmd.params[1] == "+k" )
			{
				channel->setPassword( cmd.params[2] );
				channel->setKey( true );
			}
			else if ( cmd.params.size() != 2 && cmd.params[1] == "-k" )
			{
				// error
			}
			else
			{
				channel->setPassword("");
				channel->setKey( false );			}
		}
		else if (cmd.params.size() == 2 && cmd.params[1] == "-o")
		{
			User *todemote = getUser( cmd.params[2] );
			if ( todemote )
				channel->demoteOp( &client, todemote );
		}
		else if (cmd.params.size() == 2 && cmd.params[1] == "+o")
		{
			User *topromote = getUser( cmd.params[2] );
			if ( topromote )
				channel->promoteOp( &client, topromote );
		}
	}
	catch( const std::exception& e )
	{
		send( client.getFd(), "Use : MODE <channel> <flags>\n", 30, 0 );
		return ;
	}

}

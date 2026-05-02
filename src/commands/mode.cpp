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
		std::string	option = cmd.params[1];
		if ( option == "-i" )
			channel->setInviteMode( false );
		else if ( option == "+i")
			channel->setInviteMode( true );
		else if ( option == "-t" )
			channel->setTopicIsOnlyOp( false );
		else if ( option == "+t" )
			channel->setTopicIsOnlyOp( true );
		else if ( option == "-k" || option == "+k" )
		{	
			if ( cmd.params.size() != 3 && option == "+k" )
			{
				// error
			}
			else if ( cmd.params.size() != 3 && option == "+k" )
			{
				channel->setPassword( cmd.params[2] );
				channel->setKey( true );
			}
			else if ( cmd.params.size() != 2 && option == "-k" )
			{
				// error
			}
			else
			{
				channel->setPassword("");
				channel->setKey( false );			
			}
		}
		else if (option == "-o" || option == "+o" )
		{
			if ( cmd.params.size() != 2 )
			{	
				// error
			}
			User *user = getUser( cmd.params[2] );
			if ( option == "-o" )
			{
				if ( user )
					channel->demoteOp( &client, user );
			}
			else
			{
				if ( user )
					channel->promoteOp( &client, user );
			}
		}
		else if ( option == "-l" || option == "+l" )
		{
			if ( option == "+l" && cmd.params.size() == 3 )
			{
				channel->setUserLimit( std::atoi( cmd.params[2].c_str() ) );
			}
			else if ( option == "-l" && cmd.params.size() == 2 )
			{
				channel->setUserLimit( 0 );
			}
			else
			{
				// error
			}
		}
	}
	catch( const std::exception& e )
	{
		send( client.getFd(), "Use : MODE <channel> <flags>\n", 30, 0 );
		return ;
	}

}

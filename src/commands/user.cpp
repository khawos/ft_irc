#include "../../includes/Server.hpp"

void Server::handleUsername(User &client, Command cmd)
{
	if ( cmd.params.size() != 1 )
	{
		send(client.getFd(), "Use USER <username>\n", 21, 0);
	}
	else if ( client.isUserOk() != true )
	{
		client.setUsername(cmd.params[0]);
        client.setUserOk( true );
		if (client.isNickOk())
		{
            client.setRegistered(true);   
			sendWelcomeMessages( client );
		}
	}
	else
	{
    	std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
    	std::string errMsg = ":ircserv 462 " + nick + " :You may not reregister\r\n";
	    send(client.getFd(), errMsg.c_str(), errMsg.length(), 0);
	}
}
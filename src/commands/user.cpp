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
		std::string msg = client.getUsername() + " is set as username";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
        client.setUserOk( true );        
	}
}
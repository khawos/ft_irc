#include "../../includes/Server.hpp"

void Server::handleNick(User &client, Command cmd)
{
    if ( cmd.params.size() != 1 )
    {
		send(client.getFd(), "Use NICK <nickname>\n", 21, 0);
    }
    else if ( client.isNickOk() != true )
    {
        client.setNickname(cmd.params[0]);
        std::string msg = client.getNickname() + " is set as nickname\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);    
        client.setNickOk( true );        
    }
}
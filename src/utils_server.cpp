#include "../includes/Server.hpp"

void Server::sendWelcomeMessages(User& client) {
    
    std::string nick = client.getNickname();
    std::string user = client.getUsername();
    std::string ip = client.getIp();
    
    std::string welcomeMsg = "";
    
    welcomeMsg += ":" + _serverName + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + ip + "\r\n";

    send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.length(), 0);
}

bool    Server::isUser( std::string nickname )
{
    for ( std::map<int, User *>::const_iterator it = _users.begin() ; it != _users.end() ; it++  )
    {
        if ( it->second->getNickname() == nickname )
            return (true);
    }
    return (false);
}

User    *Server::getUser( std::string nickname ) const
{
    for ( std::map<int, User *>::const_iterator it = _users.begin() ; it != _users.end() ; it++  )
    {
        if ( it->second->getNickname() == nickname )
            return ( it->second );
    }
    return (NULL);
}


bool Server::nickIsAlreadyTaken( std::string nick )
{
	std::map<int, User *>::const_iterator	it = _users.begin();

	for ( ; it != _users.end(); it++ )
	{
		if ( it->second->getNickname() == nick )
			return ( true );
	}
	return (false);
}
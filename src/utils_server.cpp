#include "../includes/Server.hpp"

void Server::sendWelcomeMessages(User& client) {
    
    std::string nick = client.getNickname();
    std::string user = client.getUsername();
    std::string ip = client.getIp();
    
    std::string welcomeMsg = "";
    
    // 1. Les 1 codes de base
    welcomeMsg += ":" + _serverName + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + ip + "\r\n";

    // 3. Envoi d'un seul bloc (plus optimisé pour le réseau)
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
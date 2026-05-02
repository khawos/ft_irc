#include "../includes/Server.hpp"

void Server::sendWelcomeMessages(User& client) {
    
    std::string nick = client.getNickname();
    std::string user = client.getUsername();
    std::string ip = "127.0.0.1";
    
    std::string welcomeMsg = "";
    
    // 1. Les 4 codes de base
    welcomeMsg += ":" + _serverName + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + ip + "\r\n";
    welcomeMsg += ":" + _serverName + " 002 " + nick + " :Your host is " + _serverName + ", running version 1.0\r\n";
    welcomeMsg += ":" + _serverName + " 003 " + nick + " :This server was created today\r\n";
    welcomeMsg += ":" + _serverName + " 004 " + nick + " " + _serverName + " 1.0 o o\r\n";

    // 2. Le MOTD (Message of the day)
    welcomeMsg += ":" + _serverName + " 375 " + nick + " :- " + _serverName + " Message of the day - \r\n";
    welcomeMsg += ":" + _serverName + " 372 " + nick + " :- Welcome to my awesome ft_irc server!\r\n";
    welcomeMsg += ":" + _serverName + " 376 " + nick + " :End of /MOTD command.\r\n";

    // 3. Envoi d'un seul bloc (plus optimisé pour le réseau)
    send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.length(), 0);
}
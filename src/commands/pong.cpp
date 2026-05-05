#include "../../includes/Server.hpp"

void    Server::handlePing(User &client, Command cmd)
{
    if (cmd.params.empty())
    {
        std::string err = ":ircserv 409 " + client.getNickname() + " :No origin specified\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }

    std::string token = cmd.params[0];

    // 🚨 NETTOYAGE CRUCIAL : On supprime le \r final s'il est resté collé au token
    if (!token.empty() && token[token.length() - 1] == '\r') {
        token.erase(token.length() - 1);
    }

    // Format universel et ultra-simple (le plus sûr pour ft_irc)
    std::string pongMsg = "PONG :" + token + "\r\n";
    
    // Envoi au client
    send(client.getFd(), pongMsg.c_str(), pongMsg.size(), 0);

    // Debugging visuel pour toi : Les crochets te permettront de voir si le format est parfait
    std::cout << "[DEBUG] PONG envoye ! Message exact : [" << token << "]" << std::endl;
}
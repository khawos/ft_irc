#include "../../includes/Server.hpp"

void    Server::handlePing(User &client, Command cmd)
{
    if (cmd.params.empty())
    {
        std::string err = ":ircserv 409 " + client.getNickname() + " :No origin specified\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }

    std::string token = cmd.params[cmd.params.size() - 1];

    if (!token.empty() && token[token.length() - 1] == '\r') {
        token.erase(token.length() - 1);
    }
    std::string inviteMsg = "PONG: " + token;
    send(client.getFd(), inviteMsg.c_str(), inviteMsg.length(), 0);
}

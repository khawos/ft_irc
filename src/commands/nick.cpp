#include "Server.hpp"
#include <cctype>

static bool	isValidNick(const std::string& nick)
{
	if (nick.empty() || nick.size() > 9)
		return false;
	// First char must be a letter or special
	char c = nick[0];
	if (!std::isalpha(static_cast<unsigned char>(c)) &&
		c != '[' && c != ']' && c != '\\' && c != '`' &&
		c != '_' && c != '^' && c != '{' && c != '}' && c != '|')
		return false;
	// Other chars: letter, digit, special, or '-'
	for (size_t i = 1; i < nick.size(); i++)
	{
		c = nick[i];
		if (!std::isalnum(static_cast<unsigned char>(c)) &&
			c != '[' && c != ']' && c != '\\' && c != '`' &&
			c != '_' && c != '^' && c != '{' && c != '}' &&
			c != '|' && c != '-')
			return false;
	}
	return true;
}

void	Server::handleNick(Client& client, const Command& cmd)
{
	if (cmd.params.empty())
	{
		sendToClient(client, ":server 431 * :No nickname given\r\n");
		return;
	}

	const std::string&	newNick = cmd.params[0];

	if (!isValidNick(newNick))
	{
		sendToClient(client, ":server 432 * " + newNick + " :Erroneous nickname\r\n");
		return;
	}

	// Check uniqueness
	Client*	existing = findClientByNick(newNick);
	if (existing != NULL && existing != &client)
	{
		sendToClient(client, ":server 433 * " + newNick + " :Nickname is already in use\r\n");
		return;
	}

	client.setNickname(newNick);
	client.setNickOk(true);

	tryRegister(client);
}

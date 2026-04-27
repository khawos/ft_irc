#include "Command.hpp"
#include <cctype>

// Parse a raw IRC line into a Command struct.
// Format: [:prefix] COMMAND [param1 param2 ...] [:trailing with spaces]
Command	parseCommand(const std::string& line)
{
	Command	cmd;
	size_t	i = 0;

	// Ignore optional :prefix (sent by some clients/servers)
	if (i < line.size() && line[i] == ':')
	{
		while (i < line.size() && line[i] != ' ')
			i++;
		while (i < line.size() && line[i] == ' ')
			i++;
	}

	// recuper le nom de la commande
	while (i < line.size() && line[i] != ' ')
	{
		cmd.name += static_cast<char>(std::toupper(static_cast<unsigned char>(line[i]))); // obliger de cast en unsigned int pour eviter les bugs
		i++;
	}

	// Extract parameters
	while (i < line.size())
	{
		while (i < line.size() && line[i] == ' ') // skip spaces
			i++;
		if (i >= line.size())
			break;

		// Trailing parameter: commence avec ':'. tout ce qui vient apres est un seul paramètre, même s'il contient des espaces
		if (line[i] == ':')
		{
			i++;
			cmd.params.push_back(line.substr(i));
			break;
		}

		// Normal parameter (no spaces)
		std::string	param;
		while (i < line.size() && line[i] != ' ')
		{
			param += line[i];
			i++;
		}
		cmd.params.push_back(param);
	}

	return cmd;
}

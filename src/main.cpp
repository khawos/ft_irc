#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>

static bool	isValidPort(const std::string& s, int& outPort)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return false;
	}
	long	val = std::atol(s.c_str());
	if (val < 1024 || val > 65535)
		return false;
	outPort = static_cast<int>(val);
	return true;
}

int	main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int	port;
	if (!isValidPort(argv[1], port))
	{
		std::cerr << "Error: port must be a number between 1024 and 65535" << std::endl;
		return 1;
	}

	std::string	password = argv[2];
	if (password.empty())
	{
		std::cerr << "Error: password cannot be empty" << std::endl;
		return 1;
	}

	try // catch les erreurs de socket(), bind() et liten()
	{
		Server	server(port, password);
		server.runServer();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}


#include "../includes/Server.hpp"
#include <iostream>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    Server server;
	int port = std::atoi(argv[1]);
    server.runServer(port);
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
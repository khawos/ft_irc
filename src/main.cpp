#include "../includes/Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int flag = 0;
	int port = std::atoi(argv[1]);
    Server server( port, flag );
    if ( flag == -1 )
    {
        std::cerr << "L'histoire de Corneille et Bernie" << std::endl;
        return ( -1 );
    }
    server.runServer();
    return 0;
}
#include "../includes/Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << "<password>" << std::endl;
        return 1;
    }
    int flag = 0;
	int port = std::atoi(argv[1]);
    std::string password( argv[2] );
    Server server( port, flag, password );
    if ( flag == -1 )
    {
        std::cerr << "L'histoire de Corneille et Bernie" << std::endl;
        return ( -1 );
    }
    server.runServer();
    return 0;
}
#include "../includes/Channel.hpp"


Channel::Channel(){}
Channel::~Channel(){}

void	Channel::addClientInChannel( const Client &newClient )
{
	switch ( _mode )
	{
		case 0:
		{
			_Clients.insert(std::make_pair(newClient.nickName, newClient));  // ✅ C++98);
			break ;
		}
	}
}

void	Channel::deleteClientFromChannel( const Client &ClientToDelete )
{
	std::string	name = ClientToDelete.nickName;
	try
	{
		_Clients.at( name );
		_Clients.erase( name );
	}
	catch(const std::exception& e)
	{
		std::cerr << name << " is not a member of " << this->_name << '\n';
	}
}

std::string	Channel::getOperator( void ) const
{
	return ( _operator );
}
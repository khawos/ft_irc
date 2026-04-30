#include "../includes/Channel.hpp"

Channel::Channel() : _fd(-1) {}

Channel::Channel( std::string op, std::string pass, std::string name) : _fd(fd), _operator(op), _password(pass),  _name(name)
{
}

Channel::~Channel(){}

void	Channel::addUserInChannel( const User &newUser )
{
	switch ( _mode )
	{
		case 0:
		{
			_Users[ newUser.getUsername()] = newUser;			
			break ;
		}
	}
}

void	Channel::deleteUserFromChannel( const User &userToDelete )
{
	std::string	name = userToDelete.getNickname();
	try
	{
		_Users.at( name );
		_Users.erase( name );
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
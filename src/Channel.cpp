#include "../includes/Channel.hpp"

Channel::Channel(){}

Channel::Channel( std::string op, std::string pass, std::string name) :  _operator(op), _password(pass),  _name(name)
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

const std::string &Channel::getName() const
{
	return (_name);
}

const std::map<std::string, User> &Channel::getUserMap() const
{
	return ( _Users );
}

void	Channel::sendMessage( const std::string &msg ) const
{
	const std::map<std::string, User>	&_User = getUserMap();
	for ( std::map<std::string, User>::const_iterator it = _User.begin(); it != _User.end(); it++ )
	{
		send(it->second.getFd(), msg.c_str(), msg.size(), 0);
	}
}
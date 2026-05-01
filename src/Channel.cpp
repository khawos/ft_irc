#include "../includes/Channel.hpp"

Channel::Channel(){}

Channel::Channel( std::string op, std::string pass, std::string name) :  _operator(op),  _name(name), _inviteMode(false), _topicIsOnlyOP(false), _key(false)
{
	_password = pass;
}

Channel::~Channel(){}

void	Channel::addUserInChannel( const User &newUser )
{
	_Users[ newUser.getUsername()] = newUser;			

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

void	Channel::setPassword( std::string str )
{
	_password = str;
	if ( str.empty() )
		_key = false;
	else
		_key = true; 
}

std::string	Channel::getPassword()
{
	return ( _password );
}

void	Channel::setInviteMode()
{
	if ( _inviteMode == false )
		_inviteMode = true;
	else
		_inviteMode = false;
}

bool	Channel::getInviteMode()
{
	return ( _inviteMode );
}

void	Channel::setTopicIsOnlyOp()
{
	if ( _topicIsOnlyOP == false )
		_topicIsOnlyOP = true;
	else
		_topicIsOnlyOP = false;
}

bool	Channel::getTopicIsOnlyOp()
{
	return ( _topicIsOnlyOP );
}

bool	Channel::getKey()
{
	return ( _key );
}
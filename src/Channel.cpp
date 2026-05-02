#include "../includes/Channel.hpp"

Channel::Channel(){}

Channel::Channel( User *op, std::string pass, std::string name) : _name(name), _topic(""), _inviteMode(false), _topicIsOnlyOP(false)
{
	_password = pass;
	if ( _password.size() != 0)
		_key = true;
	else
		_key = false;
	_operator.push_back(op);
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

bool	Channel::isOperator( const User &client ) const
{
	for (std::vector< User * >::const_iterator it = _operator.begin(); it != _operator.end(); it++ )
	{
		if ( client.getNickname() == (*it)->getNickname() )
		{
			return ( true );
		}
	} 
	return ( false );
}

const std::string &Channel::getName() const
{
	return (_name);
}

const std::map<std::string, User> &Channel::getUserMap() const
{
	return ( _Users );
}

void	Channel::sendMessage( const std::string &msg, User client ) const
{
	const std::map<std::string, User>	&_User = getUserMap();
	for ( std::map<std::string, User>::const_iterator it = _User.begin(); it != _User.end(); it++ )
	{
		if ( it->second.getNickname() != client.getNickname() )
			send(it->second.getFd(), msg.c_str(), msg.size(), 0);
	}
}

void	Channel::sendMessage_broadcast( const std::string &msg ) const
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

bool	Channel::getInviteMode() const
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

bool	Channel::getTopicIsOnlyOp() const
{
	return ( _topicIsOnlyOP );
}

bool	Channel::getKey() const
{
	return ( _key );
}

bool	Channel::isInvited( User user_ ) const
{
	try
	{
		_invitedQueueUsers.at( user_.getUsername() );
		return ( true );
	}
	catch ( std::exception &e)
	{
		return ( false );
	}
}

void	Channel::setTopic( std::string str )
{
	_topic = str;
}

std::string	Channel::getTopic()
{
	return ( _topic );
}
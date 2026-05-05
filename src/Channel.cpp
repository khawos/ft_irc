#include "../includes/Channel.hpp"

Channel::Channel(){}

Channel::Channel( User *op, std::string pass, std::string name) : _name(name), _topic(""), _inviteMode(false), _topicIsOnlyOP(false)
{
	_nbUser = 0;
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
	_Users[ newUser.getUsername() ] = newUser;			

}

void	Channel::deleteUserFromChannel( const User &userToDelete )
{
	std::string	name = userToDelete.getUsername();
	if (_Users.count(name))
		_Users.erase(name);
	else
		std::cerr << userToDelete.getNickname() << " is not a member of " << this->_name << '\n';
}

void	Channel::addToInviteList( const User &user )
{
	_invitedQueueUsers[user.getUsername()] = true; 
}

bool	Channel::isOperator( const User &client ) const
{
	for (std::vector< User * >::const_iterator it = _operator.begin(); it != _operator.end(); it++ )
	{
		if ( client.getUsername() == (*it)->getUsername() )
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
	std::cout << "Message : " <<  msg << std::endl;
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
}

std::string	Channel::getPassword()
{
	return ( _password );
}

void	Channel::setInviteMode( bool b)
{
		_inviteMode = b;
}

void	Channel::setUserLimit( int limit )
{
	_limit_user = limit;
}

unsigned int	Channel::getUserLimit() const
{
	return ( _limit_user );
}

unsigned int	Channel::getNbUser( void ) const
{
	return ( _nbUser );
}

void	Channel::incNbUSer( void )
{
	_nbUser++;
}

void	Channel::decNbUSer( void )
{
	_nbUser--;
}

void	Channel::setKey( bool b)
{
		_key = b;
}

bool	Channel::getInviteMode() const
{
	return ( _inviteMode );
}

void	Channel::setTopicIsOnlyOp( bool b)
{
	_topicIsOnlyOP = b;
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

void	Channel::promoteOp( User *admin, User *toPromote )
{
	std::string prefix = admin->getNickname() + "!" + admin->getUsername() + "@" + admin->getIp();
	std::string broadcastMsg = ":" + prefix + " MODE " + getName() + " +o " + toPromote->getNickname() + "\r\n";
	sendMessage_broadcast( broadcastMsg );
	_operator.push_back( toPromote );
}

void	Channel::demoteOp( User *admin, User *toDemote )
{
	std::string prefix = admin->getNickname() + "!" + admin->getUsername() + "@" + admin->getIp();
	std::string broadcastMsg = ":" + prefix + " MODE " + getName() + " -o " + toDemote->getNickname() + "\r\n";
	for (std::vector<User *>::iterator it = _operator.begin(); it != _operator.end() ; it++ )
	{
		if ((*it)->getNickname() == toDemote->getNickname())
		{
			_operator.erase( it );
			return ;
		}
	}

}
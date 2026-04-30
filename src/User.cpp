#include "../includes/User.hpp"

User::User(){}

User::User( unsigned int id ) : _id( id ), _readBuffer(""), _passOk( false ) {}

User::~User(){}

void	User::appendToReadBuffer(const std::string& data)
{
	_readBuffer += data;
}

bool	User::extractCommand(std::string& outLine)
{
	std::string::size_type	pos = _readBuffer.find("\r\n");
	size_t					delimLen = 2;

	if (pos == std::string::npos)
	{
		pos = _readBuffer.find("\n");
		delimLen = 1;
	}

	if (pos == std::string::npos)
		return false;

	outLine = _readBuffer.substr(0, pos);
	_readBuffer.erase(0, pos + delimLen);
	return true;
}

const std::string &User::getUsername() const
{
	return ( _username );
}

const std::string &User::getRealname() const
{
	return ( _realname );
}


const std::string &User::getNickname() const
{
	return ( _nickname );
}

const unsigned int &User::getId() const
{
	return ( _id );
}

bool User::isPassOk() const
{
	return ( _passOk );
}
bool User::isNickOk() const
{
	return ( _nickOk );
}
bool User::isUserOk() const
{
	return ( _userOk );
}

bool User::isRegistered() const
{
	return ( _registered );
}

void User::setPassOk( bool b )
{
	_passOk = b;
}
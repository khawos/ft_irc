#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd), _passOk(false), _nickOk(false), _userOk(false), _registered(false)
{}

Client::~Client() {}

// Getters
int					Client::getFd() const          { return _fd; }
const std::string&	Client::getNickname() const    { return _nickname; }
const std::string&	Client::getUsername() const    { return _username; }
const std::string&	Client::getRealname() const    { return _realname; }
bool				Client::isPassOk() const       { return _passOk; }
bool				Client::isNickOk() const       { return _nickOk; }
bool				Client::isUserOk() const       { return _userOk; }
bool				Client::isRegistered() const   { return _registered; }

// Setters
void	Client::setNickname(const std::string& nick) { _nickname = nick; }
void	Client::setUsername(const std::string& user) { _username = user; }
void	Client::setRealname(const std::string& real) { _realname = real; }
void	Client::setPassOk(bool b)                    { _passOk = b; }
void	Client::setNickOk(bool b)                    { _nickOk = b; }
void	Client::setUserOk(bool b)                    { _userOk = b; }
void	Client::setRegistered(bool b)                { _registered = b; }


void	Client::appendToReadBuffer(const std::string& data)
{
	_readBuffer += data;
}

// essayer d'extraire une ligne de commande complète (terminée par \r\n ou \n).
// la ligne extraite + le délimiteur sont supprimés du buffer 
bool	Client::extractCommand(std::string& outLine)
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

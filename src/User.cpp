#include "../includes/User.hpp"

// Constructeur par défaut : initialise TOUS les booléens (sinon mémoire aléatoire = bugs)
User::User() : _fd(-1), _passOk(false), _nickOk(false), _userOk(false), _registered(false) {}

User::User( int fd ) : _fd(fd), _readBuffer(""), _passOk(false), _nickOk(false), _userOk(false), _registered(false) {}

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

// Getters
int					User::getFd() const            { return _fd; }
const std::string&	User::getIp() const            { return _ip; }
const std::string&	User::getUsername() const      { return _username; }
const std::string&	User::getRealname() const      { return _realname; }
const std::string&	User::getNickname() const      { return _nickname; }
bool				User::isPassOk() const         { return _passOk; }
bool				User::isNickOk() const         { return _nickOk; }
bool				User::isUserOk() const         { return _userOk; }
bool				User::isRegistered() const     { return _registered; }

// Setters
void	User::setIp(const std::string& ip)         { _ip = ip; }
void	User::setNickname(const std::string& nick) { _nickname = nick; }
void	User::setUsername(const std::string& user) { _username = user; }
void	User::setRealname(const std::string& real) { _realname = real; }
void	User::setPassOk(bool b)                    { _passOk = b; }
void	User::setNickOk(bool b)                    { _nickOk = b; }
void	User::setUserOk(bool b)                    { _userOk = b; }
void	User::setRegistered(bool b)                { _registered = b; }

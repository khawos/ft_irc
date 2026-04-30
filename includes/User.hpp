#pragma once
# include <string> // std::string
# include <iostream> // std::cout
class User
{
    private : 

        unsigned int _id;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_readBuffer;	// Buffer pour read
		bool		_passOk;
		bool		_nickOk;
		bool		_userOk;
		bool		_registered;	// si les 3 du sont ok ils sont registered

    public :

        User();
        User( unsigned int id );
        ~User();

        // Getters
		int					getFd() const;
		const std::string&	getNickname() const;
		const unsigned int&	getId() const;
		const std::string&	getUsername() const;
		const std::string&	getRealname() const;
		bool				isPassOk() const;
		bool				isNickOk() const;
		bool				isUserOk() const;
		bool				isRegistered() const;

		// Setters
		void	setNickname(const std::string& nick);
		void	setUsername(const std::string& user);
		void	setRealname(const std::string& real);
		void	setPassOk(bool b);
		void	setNickOk(bool b);
		void	setUserOk(bool b);
		void	setRegistered(bool b);

		void	appendToReadBuffer(const std::string& data);
		bool	extractCommand(std::string& outLine);

};
#pragma once
# include <map> // std::map
# include <string> // std::string
# include <iostream> // std::cerr
# include "User.hpp" // User
# include <vector>


class Channel
{
	private:

		std::map<std::string, User>	_Users;
		std::map<std::string, User>	_invitedQueueUsers;
		int							_fd;
		std::string					_operator;
		std::string					_password;
		std::string					_name;
		int							_mode;


	public :
		
		Channel();
		Channel( int fd, std::string op, std::string pass, std::string name );
		~Channel();
		void		addUserInChannel( const User &newUSer );
		void		deleteUserFromChannel( const User &newUSer );
		std::string	getOperator( void ) const;
		void		setMode( std::string c );

};
#pragma once
# include <map> // std::map
# include <string> // std::string
# include <iostream> // std::cerr
# include "User.hpp" // User
# include <vector>


class Chanel
{
	private:

		std::map<std::string, User>	_Users;
		std::map<std::string, User>	_invitedQueueUsers;
		std::string					_name;
		std::string					_password;
		std::string					_operator;


	public :
		
		Chanel();
		~Chanel();
		void		addUserInChannel( const User &newUSer );
		void		deleteUserFromChannel( const User &newUSer );
		std::string	getOperator( void ) const;
		void		setMode( std::string c );

};
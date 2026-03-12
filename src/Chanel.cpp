#include "Chanel.hpp"
#
Chanel::Chanel(){}
Chanel::~Chanel(){}

void	Chanel::addUserInChannel( const User &newUser )
{
	switch ( _mode )
	{
		case 0:
		{
			_Users.insert( {newUser.nickName, newUser} );
			break ;
		}
	}
}

void	Chanel::deleteUserFromChannel( const User &userToDelete )
{
	std::string	name = userToDelete.nickName;
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

std::string	Chanel::getOperator( void ) const
{
	return ( _operator );
}
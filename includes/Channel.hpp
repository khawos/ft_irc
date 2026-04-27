#pragma once
# include <map> // std::map
# include <string> // std::string
# include <iostream> // std::cerr
# include "Client.hpp" // Client
# include <vector>


class Channel
{
	private:

		std::map<std::string, Client>	_Clients;
		std::map<std::string, Client>	_invitedQueueClients;
		std::string					_name;
		std::string					_password;
		std::string					_operator;
		int							_mode;


	public :
		
		Channel();
		~Channel();
		void		addClientInChannel( const Client &newClient );
		void		deleteClientFromChannel( const Client &newClient );
		std::string	getOperator( void ) const;
		void		setMode( std::string c );

};
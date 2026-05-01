#pragma once
# include <map> // std::map
# include <string> // std::string
# include <iostream> // std::cerr
# include "User.hpp" // User
# include <vector>
# include <sys/socket.h> // send()


class Channel
{
	private:

		std::map<std::string, User>	_Users;
		std::map<std::string, User>	_invitedQueueUsers;
		std::string					_operator;
		std::string					_password;
		std::string					_name;
		bool						_inviteMode;
		bool						_topicIsOnlyOP;
		bool						_key;

	public :
		
		Channel();
		Channel( std::string op, std::string pass, std::string name );
		~Channel();


		void								addUserInChannel( const User &newUSer );
		void								deleteUserFromChannel( const User &newUSer );
		const std::string					&getName() const;
		std::string							getOperator( void ) const;
		const std::map<std::string, User>	&getUserMap() const;


		void								sendMessage( const std::string &msg ) const;
		void								send_relay_message( User &user );

		void								setPassword( std::string str );

		std::string								getPassword();
		void								setTopicIsOnlyOp();
		bool								getTopicIsOnlyOp();
		void								setInviteMode();
		bool								getInviteMode();
		bool								getKey();

};
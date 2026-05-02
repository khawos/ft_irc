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

		std::map<std::string, User >	_Users;
		std::map<std::string, bool>		_invitedQueueUsers;
		std::vector<User *>				_operator;
		std::string						_password;
		std::string						_name;
		std::string						_topic;

		unsigned int					_limit_user;
		unsigned int					_nbUser;

		bool							_inviteMode;
		bool							_topicIsOnlyOP;
		bool							_key;

	public :
		
		Channel();
		Channel( User *op, std::string pass, std::string name );
		~Channel();


		void								addUserInChannel( const User &newUSer );
		void								deleteUserFromChannel( const User &newUSer );
		const std::string					&getName() const;
		bool								isOperator( const User &client ) const;
		const std::map<std::string, User>	&getUserMap() const;


		void								sendMessage( const std::string &msg, User client ) const;
		void								sendMessage_broadcast( const std::string &msg ) const;
		void								send_relay_message( User &user );

		void								setPassword( std::string str );
		void								setKey( bool b );
		void								setTopic( std::string str );
		void								setUserLimit( int );
		bool								isInvited( User user ) const;
		std::string							getPassword();
		unsigned int						getNbUser( void ) const;
		void								incNbUSer(); 
		void								decNbUSer(); 
		std::string							getTopic();
		void								setTopicIsOnlyOp( bool b );
		bool								getTopicIsOnlyOp() const;
		void								setInviteMode( bool b);
		bool								getInviteMode() const;
		bool								getKey() const;
		unsigned int						getUserLimit(void) const;



		void								promoteOp( User *admin, User *toPromote );
		void								demoteOp( User *admin, User *toDemote );

};
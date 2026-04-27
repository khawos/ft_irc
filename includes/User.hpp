#pragma once
# include <string> // std::string

class User
{
    private : 

        unsigned int _id;

    public :

        User();
        User( unsigned int id );
        ~User();
        std::string nickName;
        std::string userName;
};
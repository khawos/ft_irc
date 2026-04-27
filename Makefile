NAME		= ircserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g

SRCDIR		= src
INCDIR		= includes
OBJDIR		= objs

SRCS		= main.cpp \
			  Server.cpp \
			  Client.cpp \
			  Command.cpp \
			  commands/pass.cpp \
			  commands/nick.cpp \
			  commands/user.cpp \
			  commands/ping.cpp \
			  commands/quit.cpp

OBJS		= $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

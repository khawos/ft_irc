CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 

OBJ_DIR = obj

# Liste manuelle de tous les .cpp (chemins relatifs à src/)
SRC = main.cpp \
	Server.cpp \
	User.cpp \
	Command.cpp \
	Channel.cpp \
	commands/user.cpp \
	commands/topic.cpp \
	commands/mode.cpp \
	commands/privmsg.cpp \
	commands/pass.cpp \
	commands/kick.cpp \
	commands/nick.cpp \
	commands/join.cpp \
	commands/invite.cpp \
	commands/pong.cpp \
	utils_server.cpp

# Sources avec préfixe src/
SRCS := $(addprefix src/,$(SRC))

OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = ircserv

all: $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
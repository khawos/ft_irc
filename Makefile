CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj

SRC := $(shell find src -maxdepth 1 -name '*.cpp')
OBJ := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
TARGET = ft_irc

all: $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
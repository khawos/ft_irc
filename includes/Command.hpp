
#pragma once
#include <vector> // <vector>
#include <string> // std::string

struct Command {
    std::string name;
    std::vector<std::string> params;

};
	struct Command parseCommand(const std::string& line);


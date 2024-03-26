#pragma once

#include "Command.hpp"
#include <string>
#include <vector>

class Username: public Command {
	public:
		Username();
		~Username();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
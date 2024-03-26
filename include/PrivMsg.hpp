#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class PrivMsg: public Command {
	public:
		PrivMsg();
		~PrivMsg();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
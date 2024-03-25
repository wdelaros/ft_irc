#pragma once

#include "Command.hpp"

class PrivMsg: public Command {
	public:
		PrivMsg();
		~PrivMsg();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
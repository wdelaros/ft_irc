#pragma once

#include "Command.hpp"

class Nickname: public Command {
	public:
		Nickname();
		~Nickname();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
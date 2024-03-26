#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Nickname: public Command {
	public:
		Nickname();
		~Nickname();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
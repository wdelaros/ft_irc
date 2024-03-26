#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Pass: public Command {
	public:
		Pass();
		~Pass();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
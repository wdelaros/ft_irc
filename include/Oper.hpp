#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Oper: public Command {
	public:
		Oper();
		~Oper();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
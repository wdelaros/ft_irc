#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Part: public Command {
	public:
		Part();
		~Part();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Kick: public Command {
	public:
		Kick();
		~Kick();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
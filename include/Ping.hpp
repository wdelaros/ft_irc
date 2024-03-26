#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Ping: public Command {
	public:
		Ping();
		~Ping();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
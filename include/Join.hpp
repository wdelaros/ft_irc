#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Join: public Command {
	public:
		Join();
		~Join();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
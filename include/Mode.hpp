#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Mode: public Command {
	public:
		Mode();
		~Mode();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
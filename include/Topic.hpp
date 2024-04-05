#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Topic: public Command {
	public:
		Topic();
		~Topic();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
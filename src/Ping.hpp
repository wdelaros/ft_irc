#pragma once

#include "Command.hpp"

class Ping: public Command {
	public:
		Ping();
		~Ping();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
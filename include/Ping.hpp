#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Ping: public Command {
	private:
		std::string _name;

	public:
		Ping();
		~Ping();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
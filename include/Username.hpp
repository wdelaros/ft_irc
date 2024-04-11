#pragma once

#include "Command.hpp"
#include <string>
#include <vector>

class Username: public Command {
	private:
		std::string _name;

	public:
		Username();
		~Username();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
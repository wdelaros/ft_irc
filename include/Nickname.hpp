#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Nickname: public Command {
	private:
		std::string _name;

	public:
		Nickname();
		~Nickname();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
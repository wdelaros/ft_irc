#pragma once

#include "Command.hpp"

class Part: public Command {
	private:
		std::string _name;

	public:
		Part();
		~Part();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
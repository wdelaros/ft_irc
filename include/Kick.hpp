#pragma once

#include "Command.hpp"

class Kick: public Command {
	private:
		std::string _name;

	public:
		Kick();
		~Kick();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
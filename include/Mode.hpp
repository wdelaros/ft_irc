#pragma once

#include "Command.hpp"

class Mode: public Command {
	private:
		std::string _name;

	public:
		Mode();
		~Mode();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
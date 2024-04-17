#pragma once

#include "Command.hpp"

class List: public Command {
	private:
		std::string _name;

	public:
		List();
		~List();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};

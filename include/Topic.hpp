#pragma once

#include "Command.hpp"

class Topic: public Command {
	private:
		std::string _name;

	public:
		Topic();
		~Topic();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
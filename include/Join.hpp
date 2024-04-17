#pragma once

#include "Command.hpp"

class Join: public Command {
	private:
		std::string _name;

	public:
		Join();
		~Join();

		const std::string& getName() const;

		void CreateChannel(Server& server, User& eventUser, const std::string& channelName) const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class Join: public Command {
	public:
		Join();
		~Join();

		void CreateChannel(Server& server, User& eventUser, const std::string& channelName) const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
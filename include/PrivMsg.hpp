#pragma once

#include "Command.hpp"
#include <iostream>
#include <string>
#include <vector>

class PrivMsg: public Command {
	public:
		PrivMsg();
		~PrivMsg();

		std::string sendPrivMsg(Server& server, const std::string& msg, const std::string& nickname) const;
		std::string sendChannelMsg(Server& server, User& user, const std::string& msg, const std::string& channelName) const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
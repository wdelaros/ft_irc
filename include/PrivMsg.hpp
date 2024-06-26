#pragma once

#include "Command.hpp"

class PrivMsg: public Command {
	private:
		std::string _name;

	public:
		PrivMsg();
		~PrivMsg();

		const std::string& getName() const;

		std::string sendPrivMsg(Server& server, const std::string& msg, const std::string& nickname) const;
		std::string sendChannelMsg(Server& server, User& user, const std::string& msg, const std::string& channelName) const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;
};
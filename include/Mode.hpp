#pragma once

#include "Command.hpp"
#include "Channel.hpp"

class Mode: public Command {
	private:
		std::string _name;

	public:
		Mode();
		~Mode();

		const std::string& getName() const;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const;

		std::string invitationMode(Channel* chan, User& eventUser, char modif) const;
		std::string restrictionTopicMode(Channel* chan, User& eventUser, char modif) const;
		std::string passwordMode(Channel* chan, User& eventUser, char modif, std::string passw) const;
		std::string privilegeMode(Channel* chan, User& eventUser, char modif, User& target) const;
		std::string limitMode(Channel* chan, User& eventUser, char modif, int limit) const;

		std::string parseMode(Channel* channel, User& eventUser, const std::string& mode, std::string arg) const;
};

#include "../include/Username.hpp"
#include "../include/Server.hpp"

Username::Username() {

}

Username::~Username() {

}

std::string Username::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	(void)server;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	eventUser.setUsername(buffer.substr(0, buffer.find_first_of(" \r\n")));
	if (eventUser.getUsername().empty())
		msg = ERR_NOTREGISTERED((std::string)"You are not registered. Give a username (/set irc.server.<server name>.username <username>).");
	return msg;
}
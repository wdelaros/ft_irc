#include "../include/Username.hpp"
#include "../include/Server.hpp"

Username::Username(): _name("user") {

}

Username::~Username() {

}

const std::string& Username::getName() const {
	return _name;
}

std::string Username::execute(Server& server, User& eventUser, std::string& buffer) const {
	(void)server;
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 5)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);
	else if (vec.size() > 5)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many parameters");

	if (eventUser.getIsAuth())
		return ERR_ALREADYREGISTRED(eventUser.getNickname());
	if (vec[2] != "0" || vec[3] != "*" || vec[4][0] != ':') {
		if (eventUser.getUsername().empty())
			return ERR_NOTREGISTERED((std::string)"You are not registered. Give a username (USER <username> 0 * :<realname>).");
		else
			return ERR_NOTREGISTERED((std::string)"You are not registered. Give a nickname (NICK <nickname>).");
	}
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	eventUser.setUsername(buffer.substr(0, buffer.find_first_of(" \r\n")));
	if (eventUser.getUsername().empty())
		msg = ERR_NOTREGISTERED((std::string)"You are not registered. Give a username (/set irc.server.<server name>.username <username>).");
	return msg;
}
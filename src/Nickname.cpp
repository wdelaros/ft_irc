#include "../include/Nickname.hpp"
#include "../include/Server.hpp"

Nickname::Nickname(): _name("nick") {

}

Nickname::~Nickname() {

}

const std::string& Nickname::getName() const {
	return _name;
}

bool isValidStr(const std::string& buffer, bool isAlphaNum, const std::string& other) {
	if (isdigit(buffer[0]))
		return false;
	for (size_t i = 0; i < buffer.size(); i++) {
		if (isAlphaNum == 0 && other.find(buffer[i]) == std::string::npos && !isalnum(buffer[i]))
			return false;
		else if (isAlphaNum == 1 && other.find(buffer[i]) == std::string::npos && !isalpha(buffer[i]))
			return false;
	}
	return true;
}

// client send(NICK <new nickname>) | server send(:<nickname> NICK :<new nickname>)
std::string Nickname::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2) {
		msg = ERR_NONICKNAMEGIVEN(eventUser.getNickname());
		if (!eventUser.getIsAuth() && eventUser.getNickname().empty())
			return msg + ERR_NOTREGISTERED((std::string)"You are not registered. Give a nickname (NICK <nickname>).");
		else
			return msg + ERR_NOTREGISTERED((std::string)"You are not registered.  Give a username (USER <username> 0 * :<realname>).");
	}
	else if (vec.size() > 2)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many parameters");

	if (vec[1].length() > 9)
		msg = ERR_ERRONEUSNICKNAME(vec[1], "Nickname too long!");
	else if (!isValidStr(vec[1], 0, "[]`{}|\\^_^") || vec[1] == "user")
		msg = ERR_ERRONEUSNICKNAME(vec[1], "Nickname invalid character!");
	else if (server.nicknameInUse(vec[1]))
		msg = ERR_NICKNAMEINUSE(vec[1]);
	else if (!eventUser.getIsAuth())
		eventUser.setNickname(vec[1]);
	else {
		msg = ":" + eventUser.getNickname() + " NICK :" + vec[1] + "\r\n";
		for (std::map<int, User*>::const_iterator it = server.getUserList().begin(); it != server.getUserList().end(); it++)
			if (it->second->getIsAuth())
				send(it->second->getFd(), msg.c_str(), msg.size(), 0);
		msg = "";
		eventUser.setNickname(vec[1]);
	}
	if (eventUser.getNickname().empty())
		msg += ERR_NOTREGISTERED((std::string)"You are not registered. Give a nickname (/set irc.server.<server name>.nicks <nickname>).");
	return msg;
}

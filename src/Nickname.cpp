#include "../include/Nickname.hpp"
#include "../include/Server.hpp"
#include <cctype>
#include <cstddef>
#include <map>
#include <string>
#ifdef __apple
# include <_ctype.h>
#endif

Nickname::Nickname() {

}

Nickname::~Nickname() {

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
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	if (buffer.length() > 9)
		msg = "432 '" + buffer + "' :Nickname too long!\r\n";
	else if (!isValidStr(buffer, 0, "[]`{}|\\^_^"))
		msg = "432 '" + buffer + "' :Nickname invalid character!\r\n";
	else if (server.nicknameInUse(buffer))
		msg = "433 '" + buffer + "' :Nickname already in use!\r\n";
	else if (!eventUser.getIsAuth())
		eventUser.setNickname(buffer);
	else {
		msg = ":" + eventUser.getNickname() + " NICK :" + buffer + "\r\n";
		for (std::map<int, User*>::const_iterator it = server.getUserList().begin(); it != server.getUserList().end(); it++)
			send(it->second->getFd(), msg.c_str(), msg.size(), 0);
		msg = "";
		eventUser.setNickname(buffer);
	}
	if (eventUser.getNickname().empty())
		msg += "451 PRIVMSG You are not registered. Give a nickname (/set irc.server.<server name>.nicks <nickname>).\r\n";
	return msg;
	
}

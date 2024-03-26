#include "../include/Nickname.hpp"
#include "../include/Server.hpp"

Nickname::Nickname() {

}

Nickname::~Nickname() {

}

std::string Nickname::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	if (buffer.length() > 9)
		msg =  "432 PRIVMSG Nickname too long!\r\n";
	if (server.nicknameInUse(buffer))
		msg =  "433 PRIVMSG Nickname already in use!\r\n";
	if (msg.empty() && !eventUser.getIsAuth())
		eventUser.setNickname(buffer);
	else if (msg.empty()) {
		msg = ":" + eventUser.getNickname() + " NICK :" + buffer + "\r\n";
		eventUser.setNickname(buffer);
	}
	if (eventUser.getNickname().empty())
		msg += "451 PRIVMSG You are not registered. Give a nickname (/set irc.server.<server name>.nicks <nickname>).\r\n";
	return msg;
}

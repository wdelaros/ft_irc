#include "../include/PrivMsg.hpp"
#include "../include/Server.hpp"

PrivMsg::PrivMsg() {

}

PrivMsg::~PrivMsg() {

}

std::string PrivMsg::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	msg = ":" + eventUser.getNickname() + " PRIVMSG " + buffer.substr(0, buffer.find_first_of(" ")) + " " + buffer.substr(buffer.find_last_of(":")) + "\r\n";
	return server.sendPrivMsg(msg, buffer.substr(0, buffer.find_first_of(" ")));
}

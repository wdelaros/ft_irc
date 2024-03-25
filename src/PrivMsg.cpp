#include "PrivMsg.hpp"

PrivMsg::PrivMsg() {

}

PrivMsg::~PrivMsg() {

}

std::string PrivMsg::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	msg = ":" + eventUser.getNickname() + " PRIVMSG " + buffer.substr(0, buffer.find_first_of(" ")) + " " + buffer.substr(buffer.find_last_of(":")) + "\r\n";
	return server.sendPrivMsg(msg, buffer.substr(0, buffer.find_first_of(" ")));
}

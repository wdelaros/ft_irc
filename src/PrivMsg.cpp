#include "../include/PrivMsg.hpp"
#include "../include/Server.hpp"
#include <sys/socket.h>

PrivMsg::PrivMsg() {

}

PrivMsg::~PrivMsg() {

}

std::string PrivMsg::sendPrivMsg(Server& server, const std::string& msg, const std::string& nickname) const {
	int fd = server.findNickFd(nickname);
	if (fd == -1)
		return ("401 PRIVMSG " + nickname + " not found!\r\n");
	send(fd, msg.c_str(), msg.size(), 0);
	return "";
}

// client send(PRIVMSG <target> :<msg>) | server send(:<host> PRIVMSG <target> :<msg>)
std::string PrivMsg::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	std::string target = buffer.substr(0, buffer.find_first_of(" "));
	buffer = buffer.substr(buffer.find_first_of(" ") + 1);
	msg = ":" + eventUser.getNickname() + " PRIVMSG " + target + " " + buffer + "\r\n";
	if (target[0] == '#')
		return sendPrivMsg(server, msg, "frog");
	else
		return sendPrivMsg(server, msg, target);
}

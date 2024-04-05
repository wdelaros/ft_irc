#include "../include/PrivMsg.hpp"
#include "../include/Server.hpp"
#include <string>
#include <sys/socket.h>
#include <vector>

PrivMsg::PrivMsg() {

}

PrivMsg::~PrivMsg() {

}

std::string PrivMsg::sendChannelMsg(Server& server, User& user, const std::string& msg, const std::string& channelName) const {
	if (server.isChannelExist(channelName)) {
		Channel* channel = server.getChannel(channelName);
		if (channel->isUserInChannel(user.getNickname()))
			channel->sendMsg(user, msg);
		else
			return ERR_USERNOTINCHANNEL(user.getNickname(), channelName);
	}
	return "";
}

std::string PrivMsg::sendPrivMsg(Server& server, const std::string& msg, const std::string& nickname) const {
	int fd = server.findNickFd(nickname);
	if (fd == -1)
		return ERR_NOSUCHNICK(nickname);
	send(fd, msg.c_str(), msg.size(), 0);
	return "";
}

// client send(PRIVMSG <target> :<msg>) | server send(:<host> PRIVMSG <target> :<msg>)
std::string PrivMsg::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	std::string target = vec[1];
	msg = ":" + eventUser.getNickname() + " PRIVMSG " + target + " " + buffer.substr(buffer.find_first_of(":")) + "\r\n";
	if (target[0] == '#' || target[0] == '&')
		return sendChannelMsg(server, eventUser, msg, target);
	else
		return sendPrivMsg(server, msg, target);
}

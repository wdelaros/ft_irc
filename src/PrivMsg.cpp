#include "../include/PrivMsg.hpp"
#include "../include/Server.hpp"

PrivMsg::PrivMsg(): _name("privmsg") {

}

PrivMsg::~PrivMsg() {

}

const std::string& PrivMsg::getName() const {
	return _name;
}

std::string PrivMsg::sendChannelMsg(Server& server, User& user, const std::string& msg, const std::string& channelName) const {
	if (server.isChannelExist(channelName)) {
		Channel* channel = server.getChannel(channelName);
		if (channel->isUserInChannel(user.getNickname()))
			channel->sendMsg(user, msg);
		else
			return ERR_CANNOTSENDTOCHAN(user.getNickname(), channelName);
	}
	else
		return ERR_NOSUCHCHANNEL(channelName);
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

	if (vec.size() < 2)
		return ERR_NORECIPIENT(eventUser.getNickname(), vec[0]);
	else if (vec.size() == 2)
		return ERR_NOTEXTTOSEND(eventUser.getNickname(), "No text to send!");
	else if (vec.size() > 3)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Too many parameters");
	else if (vec.size() > 2 && vec[2][0] != ':')
		return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Use (PRIVMSG <target> :<message>)");

	std::string target = vec[1];
	msg = ":" + eventUser.getNickname() + " PRIVMSG " + target + " " + vec[2] + "\r\n";
	if (target[0] == '#' || target[0] == '&')
		return sendChannelMsg(server, eventUser, msg, target);
	else
		return sendPrivMsg(server, msg, target);
}

// for (size_t i = 0; i < vec.size(); i++)
// 	std::cout << vec[i] << std::endl;
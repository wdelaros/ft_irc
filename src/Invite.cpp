#include "../include/Invite.hpp"
#include "../include/Server.hpp"

Invite::Invite(): _name("invite") {

}

Invite::~Invite() {

}

const std::string& Invite::getName() const {
	return _name;
}

// client send(INVITE <target> <channel>) | server send(:<nickname> INVITE <target> <channel>)
std::string Invite::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 3)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);
	else if (vec.size() > 3)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many parameters");
	else if (buffer.find_first_of(":,") != std::string::npos)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Invalid character");

	if (!server.nicknameInUse(vec[1]) || !server.nickIsAuth(vec[1]))
		return ERR_NOSUCHNICK(vec[1]);

	if (server.isChannelExist(vec[2])) {
		Channel *channel = server.getChannel(vec[2]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (channel->getMode('i')) {
				if (channel->getIsOp(eventUser)) {
					if (!channel->isUserInChannel(vec[1])) {
						msg = ":" + eventUser.getNickname() + " INVITE " + vec[1] + " " + vec[2] + "\r\n";
						channel->addInvite(server.getUserList().at(server.findNickFd(vec[1])));
						send(server.findNickFd(vec[1]), msg.c_str(), msg.length(), 0);
						msg = RPL_INVITING(eventUser.getNickname(), vec[1], vec[2]);
					}
					else
						return ERR_USERONCHANNEL(eventUser.getNickname(), vec[2], vec[1]);
				}
				else
					return ERR_CHANOPRIVSNEEDED(vec[2]);
			}
			else {
				if (!channel->isUserInChannel(vec[1])) {
					msg = ":" + eventUser.getNickname() + " INVITE " + vec[1] + " " + vec[2] + "\r\n";
					channel->addInvite(server.getUserList().at(server.findNickFd(vec[1])));
					send(server.findNickFd(vec[1]), msg.c_str(), msg.length(), 0);
					msg = RPL_INVITING(eventUser.getNickname(), vec[1], vec[2]);
				}
				else
					return ERR_USERONCHANNEL(eventUser.getNickname(), vec[2], vec[1]);
			}
		}
		else
			return ERR_NOTONCHANNEL(eventUser.getNickname(), vec[2]);
	}
	else {
		msg = ":" + eventUser.getNickname() + " INVITE " + vec[1] + " " + vec[2] + "\r\n";
		send(server.findNickFd(vec[1]), msg.c_str(), msg.length(), 0);
		msg = RPL_INVITING(eventUser.getNickname(), vec[1], vec[2]);
	}
	return msg;
}
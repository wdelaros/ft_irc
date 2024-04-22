#include "../include/Kick.hpp"
#include "../include/Server.hpp"

Kick::Kick(): _name("kick") {

}

Kick::~Kick() {

}

const std::string& Kick::getName() const {
	return _name;
}

// client send(KICK <channel> <victim> :<message>) | server send(:<nickname> KICK <channel> <victim> :<message>)
std::string Kick::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 3)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);
	
	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (channel->getIsOp(eventUser)) {
				if (channel->isUserInChannel(vec[2])) {

				}
				else
					return ERR_USERNOTINCHANNEL(eventUser.getNickname(), channel->getName(), vec[2]);
			}
			else
				return ERR_CHANOPRIVSNEEDED(vec[1]);
		}
		else
			return ERR_NOTONCHANNEL(eventUser.getNickname(), vec[1]);
	}
	else
		return ERR_NOSUCHCHANNEL(vec[1]);

	return msg;
}
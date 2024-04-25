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
	else if (vec.size() == 4 && vec[3][0] != ':')
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Use (KICK <channel> <victim> :<message>) or (KICK <channel> <victim>)");
	else if (vec.size() > 4)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many parameter");
	
	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (channel->getIsOp(eventUser)) {
				if (channel->isUserInChannel(vec[2])) {
					if (vec.size() == 4)
						channel->KickUser(&eventUser, &channel->getUser(vec[2]), vec[3].substr(1));
					else
						channel->KickUser(&eventUser, &channel->getUser(vec[2]), "has been kick");
					if (!channel->getUserCount())
						server.deleteChannel(channel->getName());
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
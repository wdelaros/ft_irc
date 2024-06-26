#include "../include/Topic.hpp"
#include "../include/Server.hpp"

Topic::Topic(): _name("topic") {

}

Topic::~Topic() {

}

const std::string& Topic::getName() const {
	return _name;
}

// client send(TOPIC <channel> :<topic>) | server send(:<nickname> TOPIC <channel> :<topic>)
std::string Topic::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);
	else if (vec.size() == 3 && vec[2][0] != ':')
			return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Use (TOPIC <channel> :<topic>)");
	else if (vec.size() > 3)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Use (TOPIC <channel> :<topic>)");

	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() < 3)
				channel->sendTopic(&eventUser);
			else if (channel->getMode('t')) {
				if (channel->getIsOp(eventUser)) {
					channel->setTopic(vec[2].substr(1));
					channel->sendBroadcastAll(":" + eventUser.getNickname() + " " + RPL_TOPIC(eventUser.getNickname(), channel->getName(), channel->getTopic()));
				}
				else
					msg = ERR_CHANOPRIVSNEEDED(channel->getName());
			}
			else {
				channel->setTopic(vec[2].substr(1));
				channel->sendBroadcastAll(":" + eventUser.getNickname() + " " + RPL_TOPIC(eventUser.getNickname(), channel->getName(), channel->getTopic()));
			}
		}
		else
			msg = ERR_NOTONCHANNEL(eventUser.getNickname(), channel->getName());
	}
	else
		msg = ERR_NOSUCHCHANNEL(vec[1]);
	return msg;
}

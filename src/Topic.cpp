#include "../include/Topic.hpp"
#include "../include/Server.hpp"

Topic::Topic() {

}

Topic::~Topic() {

}

// client send(TOPIC <channel> :<topic>) | server send(:<nickname> TOPIC <channel> :<topic>)
std::string Topic::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), buffer);
	else if (vec.size() > 3)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Use (TOPIC <channel> :<topic>)");

	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() < 3)
				channel->sendTopic(&eventUser); //not finish
			else if (channel->getMode().find("t") != std::string::npos) {
				if (channel->getIsOp(eventUser))
					channel->setTopic(vec[2].substr(1));
				else
					msg = ERR_CHANOPRIVSNEEDED(channel->getName());
			}
			else
				channel->setTopic(vec[2].substr(1));
		}
		else
			msg = ERR_USERNOTINCHANNEL(eventUser.getNickname(), channel->getName());
	}
	else
		msg = ERR_NOSUCHCHANNEL(vec[1]);
	return msg;
}

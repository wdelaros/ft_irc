#include "../include/Topic.hpp"
#include "../include/Server.hpp"

Topic::Topic() {

}

Topic::~Topic() {

}

// client send(TOPIC <channel> <topic>) | server send(:<nickname> TOPIC <channel> <topic>)
std::string Topic::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);

	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() < 3)
				msg = channel->getTopic();
			else if (channel->getMode().find("t") != std::string::npos) {
				if (channel->getIsOp(eventUser))
					channel->setTopic(vec[2]);
				else
					msg = ERR_CHANOPRIVSNEEDED(channel->getName());
			}
			else
				channel->setTopic(vec[2]); //to change
		}
		else
			msg = ERR_USERNOTINCHANNEL(eventUser.getNickname(), channel->getName());
	}
	else
		msg = ERR_NOSUCHCHANNEL(buffer.substr(0, buffer.find_first_of(" \r\n")));
	return msg;
}

//topic msg is_print();
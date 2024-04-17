#include "../include/Part.hpp"
#include "../include/Server.hpp"

Part::Part(): _name("part") {

}

Part::~Part() {

}

const std::string& Part::getName() const {
	return _name;
}

// client send(PART <channel> :<message>) | server send(:<nickname> PART <channel> :<message>)
std::string Part::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), buffer);
	if (vec.size() > 2) {
		if (vec[2][0] != ':')
			return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Use (PART <channel> :<message>) or (PART <channel>)");
		if (vec.size() > 3)
			return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Too many parameters");
	}

	if (server.isChannelExist(vec[1])) {
		Channel *channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() > 2)
				channel->disconnectUser(&eventUser, vec[2]);
			else
				channel->disconnectUser(&eventUser, "bye");
			std::cout << "Channel: " << channel->getName() << "	user connected: " << channel->getUserCount() << std::endl;
			if (!channel->getUserCount())
				server.deleteChannel(channel->getName());
		}
		else
			msg = ERR_NOTONCHANNEL(eventUser.getNickname(), vec[1]);
	}
	else
		return ERR_NOSUCHCHANNEL(vec[1]);
	return msg;
}
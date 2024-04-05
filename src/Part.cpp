#include "../include/Part.hpp"
#include "../include/Server.hpp"
#include <cstddef>

Part::Part() {

}

Part::~Part() {

}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Part::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	Channel *channel = server.getChannel(vec[1]);
	if (channel->isUserInChannel(eventUser.getNickname())) {
		channel->disconnectUser(&eventUser, buffer.substr(buffer.find_first_of(":") + 1));
		std::cout << "Channel: " << channel->getName() << "	user connected: " << channel->getUserCount() << std::endl;
		if (!channel->getUserCount())
			server.deleteChannel(channel->getName());
	}
	else
		msg = ERR_USERNOTINCHANNEL(eventUser.getNickname(), vec[1]);
	return msg;
}
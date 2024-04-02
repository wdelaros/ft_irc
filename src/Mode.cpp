#include "../include/Mode.hpp"
#include "../include/Server.hpp"
#include <vector>

Mode::Mode() {

}

Mode::~Mode() {

}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Mode::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);

	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (eventUser.getIsOp() || channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() > 2) {
				// msg = "482 '" + buffer.substr(0, buffer.find_first_of(" \r\n")) + "' :You are not operator!\r\n";
				;
			}
			else
				channel->sendMode(eventUser);
		}
		else
			msg = "441 " + eventUser.getNickname() + " " + channel->getName() + " :No such channel!\r\n";
	}
	else
		msg = "403 '" + buffer.substr(0, buffer.find_first_of(" \r\n")) + "' :No such channel!\r\n";
	return msg;
}

// i	invitation
// t	restiction topic op
// k	password(key)
// o	privilege op
// l	limit user

// IRC Operator = god
// Channel Operator = jesus

// IRC Operator can force change channel mode

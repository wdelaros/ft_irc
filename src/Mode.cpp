#include "../include/Mode.hpp"
#include "../include/Server.hpp"
#include <cstddef>
#include <vector>

Mode::Mode() {

}

Mode::~Mode() {

}

int checkMode(const std::string& mode, const std::string& delimiter) {
	if (mode[0] != '+' && mode[0] != '-')
		return false;
	for (size_t i = 1; i < mode.size(); i++) {
		if ((mode[i - 1] == '-' || mode[i - 1] == '+') && (mode[i] == '+' || mode[i] == '-'))
			return false;
		if (delimiter.find(mode[i]) == std::string::npos && mode[i] != '+' && mode[i] != '-')
			return false;
	}
	return true;
}

std::string parseMode(Channel* channel, const std::string& mode) {
	std::string msg;

	if (channel->getMode().find("k") != std::string::npos && mode.find("k") != std::string::npos) {
		if (mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = "467 " + channel->getName() + " :Channel key already set\r\n";
	}
	return msg;
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
				if (channel->getUserList()[&eventUser] || eventUser.getIsOp()) {
					if (checkMode(vec[2], "itkol"))
						msg = parseMode(channel, vec[2]);
					else
						msg = "472 " + vec[2] + " :is unknown mode char to me for " + vec[1] + "\r\n";
				}
				else
					msg = "482 '" + buffer.substr(0, buffer.find_first_of(" \r\n")) + "' :You're not channel operator!\r\n";
			}
			else
				channel->sendMode(eventUser);
		}
		else
			msg = "441 " + eventUser.getNickname() + " " + channel->getName() + " :You're not in this channel!\r\n";
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


// oper <nickname> <password>
// IRC Operator = god
// Channel Operator = jesus

// IRC Operator can force change channel mode

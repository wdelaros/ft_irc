#include "../include/Join.hpp"
#include "../include/Server.hpp"
#include <algorithm>
#include <regex>
#include <string>
#include <vector>

Join::Join() {

}

Join::~Join() {

}

void Join::CreateChannel(Server& server, User& eventUser, const std::string& channelName) const {
	std::string msg;
	Channel* channel = new Channel(channelName, "", &eventUser);
	server.addChannel(channelName, channel);
	msg = ":" + eventUser.getNickname() + " JOIN " + channelName + "\r\n";
	send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
	channel->sendUserList(&eventUser);
}

void Join::CreateChannel(Server& server, User& eventUser, const std::string& channelName, const std::string& key) const {
	std::string msg;
	Channel* channel = new Channel(channelName, key, &eventUser);
	server.addChannel(channelName, channel);
	msg = ":" + eventUser.getNickname() + " JOIN " + channelName + "\r\n";
	send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
	channel->sendUserList(&eventUser);
}

std::vector<std::string> tokenize(const std::string& buffer, const std::string& delimiter) {
	std::vector<std::string> vector;
	std::string token;
	size_t startPos = 0;
	size_t endPos = buffer.find_first_of(delimiter);

	while (endPos != std::string::npos) {
		token = buffer.substr(startPos, endPos - startPos);
		if (token.find_first_of(delimiter) == std::string::npos)
			vector.push_back(token);
		startPos = endPos + 1;
		endPos = buffer.find_first_of(delimiter, startPos);
	}
	token = buffer.substr(startPos);
	if (token.find(delimiter) == std::string::npos)
		vector.push_back(token);
	return vector;
}

// client send(JOIN <target>[SPACE<target>]) | server send(:<nickname> JOIN <channel name>)
std::string Join::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::regex regex("(#|&)[a-zA-z]+");
	std::vector<std::string> vec = tokenize(buffer, " ");
	if (vec.size() > 0)
		std::cout << vec[0] << std::endl;
	if (vec.size() > 1)
		std::cout << vec[1] << std::endl;
	if (vec.size() > 2)
		std::cout << vec[2] << std::endl;

	if (std::regex_match(vec[1], regex)) {
		if (server.isChannelExist(vec[1].substr(0, vec[1].find_first_of(",\r\n")))) {
			Channel *channel = server.getChannel(vec[1].substr(0, vec[1].find_first_of(",\r\n")));
			if ((vec.size() > 2 && channel->getKey() == vec[2]) || (channel->getKey().empty() && vec.size() < 3)) {
				channel->addUser(&eventUser);
				msg = ":" + eventUser.getNickname() + " JOIN " + vec[1] + "\r\n";
				send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
				channel->sendBroadcastUserList();
				msg = "";
			}
			else
				msg = "475 '" + vec[1] + "' :Wrong password!\r\n";;
		}
		else {
			if (vec.size() > 2)
				CreateChannel(server, eventUser, vec[1], vec[2]);
			else
				CreateChannel(server, eventUser, vec[1]);
		}
	}
	else
		msg = "403 '" + vec[1] + "' :No such channel\r\n";
	return msg;
}

// vector			loop channel and loop key

// /nick <nick>
// <nick>		::= <letter>{<letter>|<number>|<special>}

// /join <target>[SPACE<target>]
// /ping 

// <target>		::= <to>[","<target>]
// <to>			::= <channel>|<nick>
// <channel>	::= ('#'|'&')<chstring>
// <chstring>	::= any 8bit code except SPACE, BELL, NUL, CR, LF and comma (',')

// <letter>		::= [a-zA-Z]
// <number>		::= [0-9]
// <special>	::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'



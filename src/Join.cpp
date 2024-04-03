#include "../include/Join.hpp"
#include "../include/Server.hpp"
#include <algorithm>
#include <cstddef>
#include <regex>
#include <string>
#include <utility>
#include <variant>
#include <vector>

Join::Join() {

}

Join::~Join() {

}

void Join::CreateChannel(Server& server, User& eventUser, const std::string& channelName) const {
	std::string msg;
	Channel* channel = new Channel(channelName,  &eventUser);
	server.addChannel(channelName, channel);
	msg = ":" + eventUser.getNickname() + " JOIN " + channelName + "\r\n";
	send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
	channel->sendUserList(&eventUser);
}

std::map<std::string, std::string> parseChannelKey(std::vector<std::string> channel, std::vector<std::string> key) {
	std::map<std::string, std::string> channelKey;

	for (size_t i = 0; i < channel.size(); i++) {
		if (i < key.size())
			channelKey.insert(std::pair<std::string, std::string>(channel[i], key[i]));
		else
			channelKey.insert(std::pair<std::string, std::string>(channel[i], ""));
	}
	return channelKey;
}

std::map<std::string, std::string> parseChannelKey(std::vector<std::string> channel) {
	std::map<std::string, std::string> channelKey;

	for (size_t i = 0; i < channel.size(); i++)
		channelKey.insert(std::pair<std::string, std::string>(channel[i], ""));
	return channelKey;
}

bool joinChannel(Channel* channel, User& eventUser, std::map<std::string, std::string>::iterator it) {
	std::string msg;
	if (channel->getLimitUser() < channel->getUserCount() || channel->getLimitUser() == -1) {
		channel->addUser(&eventUser);
		msg = ":" + eventUser.getNickname() + " JOIN " + it->first + "\r\n";
		channel->sendBroadcastAll(msg);
		// send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
		channel->sendBroadcastUserList();
		msg = "366 " + eventUser.getNickname() + " " + channel->getName() + " :End of /NAMES list" + "\r\n";
		send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
		return true;
	}
	return false;
}

// client send(JOIN <target>[SPACE<target>]) | server send(:<nickname> JOIN <channel name>)
std::string Join::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::regex regex("(#|&)[a-zA-z]+");
	std::vector<std::string> vec = tokenize(buffer, " ");
	std::map<std::string, std::string> channelKey;

	if (vec[1] == "0") {
		server.disconnectUserChannel(eventUser);
		return "";
	}

	if (vec.size() > 2)
		channelKey = parseChannelKey(tokenize(vec[1], ","), tokenize(vec[2], ","));
	else
		channelKey = parseChannelKey(tokenize(vec[1], ","));

	for (std::map<std::string, std::string>::iterator it = channelKey.begin(); it != channelKey.end(); it++) {
		if (std::regex_match(it->first, regex)) {
			if (server.isChannelExist(it->first.substr(0, it->first.find_first_of(",\r\n")))) {
				Channel *channel = server.getChannel(it->first.substr(0, it->first.find_first_of(",\r\n")));
				if (channel->isUserInChannel(eventUser.getNickname())) {
					msg = "443 '" + it->first + "' :You're already on that channel!\r\n";
					send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
					msg = "";
				}
				else if (eventUser.getIsOp()) {
					if (!joinChannel(channel, eventUser, it))
						msg = "471 '" + it->first + "':Cannot join channel (+l) limit: " + reinterpret_cast<const std::string&>(channel->getLimitUser()) + "\r\n";
				}
				else if (channel->getMode().find("i") != std::string::npos) {
					if (channel->getKey() == it->second || channel->isUserInInviteList(&eventUser)) {
						if (!joinChannel(channel, eventUser, it))
							msg = "471 '" + it->first + "':Cannot join channel (+l)\r\n";
					}
					else
						msg = "473 '" + it->first + "' :Cannot join channel (+i)\r\n";
				}
				else if (channel->getKey() == it->second) {
					if (!joinChannel(channel, eventUser, it))
						msg = "471 '" + it->first + "':Cannot join channel (+l)\r\n";
				}
				else {
					if (channel->getMode().find("k") == std::string::npos)
						msg = "475 '" + it->first + "' :Wrong password!\r\n";
					else
						msg = "475 '" + it->first + "' :Cannot join channel (+k)\r\n";
				}
			}
			else {
				if (it->second.empty())
					CreateChannel(server, eventUser, it->first);
				else
					msg = "403 '" + it->first + "' :No such channel\r\n";
			}
		}
		else
			msg = "403 '" + it->first + "' :No such channel\r\n";
	}
	return msg;
}

// vector			loop channel and loop key
// std::map<std::string, std::string> function(std::vector<std::string> channel = tokenize(vec[1]))
// std::map<std::string, std::string> function(std::vector<std::string> channel = tokenize(vec[1]), std::vector<std::string> key = tokenize(vec[2]))
// <channel, key>
// if !vec[2] || !vec[2][i]
// 	<channel, "">

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
// <special>	::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}' | '_' | '|'

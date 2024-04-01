#include "../include/Join.hpp"
#include "../include/Server.hpp"
#include <regex>

Join::Join() {

}

Join::~Join() {

}

void Join::CreateChannel(Server& server, User& eventUser, const std::string& channelName) const {
	std::string msg;
	Channel* channel = new Channel(channelName, &eventUser);
	server.addChannel(channelName, channel);
	msg = ":" + eventUser.getNickname() + " JOIN " + channelName + "\r\n";
	send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
	channel->sendUserList(&eventUser);
}

// client send(JOIN <channel name>) | server send(:<nickname> JOIN <channel name>)
std::string Join::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::regex regex("(#|&)[a-zA-z]+");
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);

	if (std::regex_match(buffer, regex)) {
		if (server.isChannelExist(buffer.substr(0, buffer.find_first_of(" ,\r\n")))) {
			Channel *channel = server.getChannel(buffer.substr(0, buffer.find_first_of(" ,\r\n")));
			channel->addUser(&eventUser);
			msg = ":" + eventUser.getNickname() + " JOIN " + buffer + "\r\n";
			send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
			channel->sendBroadcastUserList();
			msg = "";
		}
		else
			CreateChannel(server, eventUser, buffer);
	}
	else
		msg = "403 " + buffer + " :No such channel\r\n";
	return msg;
}

// /nick <nick>
// <nick>		::= <letter>{<letter>|<number>|<special>}

// /join <target>
// /ping 

// <target>		::= <to>[","<target>]
// <to>			::= <channel>|<nick>
// <channel>	::= ('#'|'&')<chstring>
// <chstring>	::= any 8bit code except SPACE, BELL, NUL, CR, LF and comma (',')

// <letter>		::= [a-zA-Z]
// <number>		::= [0-9]
// <special>	::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'



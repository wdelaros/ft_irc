#include "../include/Join.hpp"
#include "../include/Server.hpp"

Join::Join(): _name("join") {

}

Join::~Join() {

}

const std::string& Join::getName() const {
	return _name;
}

void Join::CreateChannel(Server& server, User& eventUser, const std::string& channelName) const {
	std::string msg;
	Channel* channel = new Channel(channelName,  &eventUser);
	server.addChannel(channelName, channel);
	msg = ":" + eventUser.getNickname() + " JOIN " + channelName + "\r\n";
	send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
	channel->sendTopic(&eventUser);
	channel->sendUserList(&eventUser);
}

std::map<std::string, std::string> parseChannelKey(std::vector<std::string> channel, std::vector<std::string> key) {
	std::map<std::string, std::string> channelKey;

	if (channel.size() < key.size())
		return channelKey;

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
	if (channel->getLimitUser() > channel->getUserCount() || channel->getLimitUser() == -1) {
		channel->addUser(&eventUser);
		msg = ":" + eventUser.getNickname() + " JOIN " + it->first + "\r\n";
		channel->sendBroadcastAll(msg);
		channel->sendTopic(&eventUser);
		channel->sendBroadcastUserList();
		msg = RPL_ENDOFNAMES(eventUser.getNickname(), channel->getName());
		send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
		return true;
	}
	return false;
}

static bool isValidStr(const std::string& buffer) {
	if (buffer == "#🦃🍎🥓🧀🐿️")
		return true;
	if (buffer[0] != '#' && buffer[0] != '&')
		return false;
	for (size_t i = 1; i < buffer.size(); i++) {
		if (!isalnum(buffer[i]) && buffer[i] != '-' && buffer[i] != '_')
			return false;
	}
	return true;
}

std::string checkError(User& eventUser, std::vector<std::string> vec) {
	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);
	else if (vec.size() < 4) {
		if (vec.size() < 3) {
			if (vec[1][0] == ',' || vec[1][vec[1].length() - 1] == ',')
				return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Use (JOIN <channel name>[,<channel name>])");
		}
		else if (vec.size() < 4) {
			if (vec[2][0] == ',' || vec[2][vec[2].length() - 1] == ',')
				return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Use (JOIN <channel name>[,<channel name>] <key>[,<key>])");
		}
	}
	else if (vec.size() > 3)
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many parameters");
	return "";
}

// client send(JOIN <target>[SPACE<target>]) | server send(:<nickname> JOIN <channel name>)
std::string Join::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	std::map<std::string, std::string> channelKey;

	msg = checkError(eventUser, vec);
	if (!msg.empty())
		return msg;

	if (vec.size() > 2)
		channelKey = parseChannelKey(tokenize(vec[1], ","), tokenize(vec[2], ","));
	else
		channelKey = parseChannelKey(tokenize(vec[1], ","));

	if (channelKey.begin() == channelKey.end())
		return ERR_UNKNOWNERROR(eventUser.getNickname(), vec[0], "Too many key");

	if (vec[1] == "0") {
		server.disconnectUserFromAllChannel(eventUser);
		return "";
	}

	for (std::map<std::string, std::string>::iterator it = channelKey.begin(); it != channelKey.end(); it++) {
		if (isValidStr(it->first)) {
			if (server.isChannelExist(it->first.substr(0, it->first.find_first_of(",\r\n")))) {
				Channel *channel = server.getChannel(it->first.substr(0, it->first.find_first_of(",\r\n")));
				if (channel->isUserInChannel(eventUser.getNickname())) {
					msg = ERR_USERONCHANNEL(eventUser.getNickname(), it->first);
					send(eventUser.getFd(), msg.c_str(), msg.size(), 0);
					msg = "";
				}
				else if (channel->getMode('i')) {
					if (channel->isUserInInviteList(&eventUser)) {
						if (channel->getKey() == it->second) {
							if (!joinChannel(channel, eventUser, it))
								msg = ERR_CHANNELISFULL(it->first);
						}
						else
							msg = ERR_BADCHANNELKEY(it->first, "Cannot join channel (+k)");
					}
					else
							msg = ERR_INVITEONLYCHAN(it->first);
				}
				else if (channel->getKey() == it->second) {
					if (!joinChannel(channel, eventUser, it))
						msg = ERR_CHANNELISFULL(it->first);
				}
				else {
					if (!channel->getMode('k'))
						msg = ERR_BADCHANNELKEY(it->first, "Wrong password!");
					else
						msg = ERR_BADCHANNELKEY(it->first, "Cannot join channel (+k)");
				}
			}
			else {
				if (it->second.empty())
					CreateChannel(server, eventUser, it->first);
				else
					msg = ERR_NOSUCHCHANNEL(it->first);
			}
		}
		else
			msg = ERR_NOSUCHCHANNEL(it->first);
	}
	return msg;
}

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

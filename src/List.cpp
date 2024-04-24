#include "../include/List.hpp"
#include "../include/Server.hpp"

List::List(): _name("list") {

}

List::~List() {

}

const std::string& List::getName() const {
	return _name;
}

// client send(LIST) | server send(<channel>)
std::string List::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::string count;
	std::vector<std::string> vec = tokenize(buffer, " ");

	for (std::map<std::string, Channel*>::const_iterator it = server.getChannelList().begin(); it != server.getChannelList().end(); it ++) {
		if (it->second->getTopic().empty())
			msg = RPL_LIST(eventUser.getNickname(), it->second->getName(), std::to_string(it->second->getUserCount()), "No topic set");
		else
			msg = RPL_LIST(eventUser.getNickname(), it->second->getName(), std::to_string(it->second->getUserCount()), it->second->getTopic());
		send(eventUser.getFd(), msg.c_str(), msg.length(), 0);
	}
	return RPL_LISTEND(eventUser.getNickname());
}

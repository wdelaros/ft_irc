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
	(void) server;
	(void) eventUser;
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	
	msg = RPL_LIST(eventUser.getNickname(), );
	send(eventUser.getFd(), msg.c_str(), msg.length(), 0);
	
	return msg;
}

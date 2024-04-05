#include "../include/Topic.hpp"
#include "../include/Server.hpp"

Topic::Topic() {

}

Topic::~Topic() {

}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Topic::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	(void) server;
	(void) eventUser;
	(void) buffer;
	return msg;
}
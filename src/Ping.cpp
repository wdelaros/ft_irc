#include "../include/Ping.hpp"
#include "../include/Server.hpp"

Ping::Ping() {

}

Ping::~Ping() {

}

// client send(PING <message>) | server send(PONG <message>)
std::string Ping::execute(Server& server, User& eventUser, std::string& buffer) const {
	(void)server;
	std::vector<std::string> vec = tokenize(buffer, " ");
	if (vec.size() < 2)
		return ERR_NOORIGIN(eventUser.getNickname());
	return ("PONG " + buffer.substr(buffer.find_first_of(" \r\n") + 1) + "\r\n");
}

#include "../include/Ping.hpp"
#include "../include/Server.hpp"

Ping::Ping() {

}

Ping::~Ping() {

}

std::string Ping::execute(Server& server, User& eventUser, std::string& buffer) const {
	(void)server;
	(void)eventUser;
	return ("PONG " + buffer.substr(buffer.find_first_of(" \r\n") + 1) + "\r\n");
}

#include "../include/Ping.hpp"
#include "../include/Server.hpp"

Ping::Ping() {

}

Ping::~Ping() {

}

// client send(PING <ip address>) | server send(PONG <ip address>)
std::string Ping::execute(Server& server, User& eventUser, std::string& buffer) const {
	(void)server;
	(void)eventUser;
	return ("PONG " + buffer.substr(buffer.find_first_of(" \r\n") + 1) + "\r\n");
}

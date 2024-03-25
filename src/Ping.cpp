#include "Ping.hpp"

Ping::Ping() {

}

Ping::~Ping() {

}

std::string Ping::execute(Server& server, User& eventUser, std::string& buffer) const {
	(void)server;
	(void)eventUser;
	return ("PONG " + buffer + "\r\n");
}

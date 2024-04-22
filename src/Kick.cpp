#include "../include/Kick.hpp"
#include "../include/Server.hpp"

Kick::Kick(): _name("kick") {

}

Kick::~Kick() {

}

const std::string& Kick::getName() const {
	return _name;
}

// client send(KICK <channel> <victim> :<message>) | server send(:<nickname> KICK <channel> <victim> :<message>)
std::string Kick::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	return msg;
}
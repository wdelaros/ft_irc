#include "../include/Kick.hpp"
#include "../include/Server.hpp"

Kick::Kick() {

}

Kick::~Kick() {

}

// client send(KICK <channel> <victim> :<message>) | server send(:<nickname> KICK <channel> <victim> :<message>)
std::string Kick::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	


	return msg;
}
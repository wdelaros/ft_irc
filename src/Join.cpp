#include "../include/Join.hpp"
#include "../include/Server.hpp"

Join::Join() {

}

Join::~Join() {

}

// client send(JOIN <channel name>) | server send(:<nickname> JOIN <channel name>)
std::string Join::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	(void)server;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);
	if (buffer[0] != '1')
		msg = ":" + eventUser.getNickname() + " JOIN " + buffer + "\r\n";
	// else {
		// msg = ":" + eventUser.getNickname() + " JOIN " + buffer.substr(1) + "\r\n";
		// send(4, msg.c_str(), msg.size(), 0);
	// }
	return msg;
}

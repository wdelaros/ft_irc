#include "../include/Mode.hpp"
#include "../include/Server.hpp"

Mode::Mode() {

}

Mode::~Mode() {

}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Mode::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	(void)eventUser;
	buffer = buffer.substr(buffer.find_first_of(" \r\n") + 1);

	if (server.isChannelExist(buffer.substr(0, buffer.find_first_of(" \r\n")))) {
			msg = "";
			// msg = "482 '" + buffer.substr(0, buffer.find_first_of(" \r\n")) + "' :You are not operator!\r\n";
	}
	else
		msg = "403 '" + buffer.substr(0, buffer.find_first_of(" \r\n")) + "' :No such channel!\r\n";
	return msg;
}

// i	invitation
// t	restiction topic op
// k	password(key)
// o	privilege op
// l	limit user

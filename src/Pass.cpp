#include "../include/Pass.hpp"
#include "../include/Server.hpp"

Pass::Pass() {

}

Pass::~Pass() {

}

std::string Pass::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	if (buffer == "PASS " + server.getPassword())
		eventUser.setHavePass(true);
	else
		msg = "451 PRIVMSG Wrong password, use /set irc.server.<server_name>.password <password>!\r\n";
	return msg;
}
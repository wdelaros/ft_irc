#include "Command.hpp"
#include "Ping.hpp"
#include "PrivMsg.hpp"
#include "Nickname.hpp"

Command::Command() {

}

Command::~Command() {

}

Command* Command::createCommand(std::vector<std::string>& line) {
	// client send(PING <ip address>) | server send(PONG <ip address>)
	if (!line.front().compare(0, 5, "PING ")) {
		line.front() = line.front().substr(line.front().find_first_of(" \r\n"));
		return new Ping();
	}
	// client send(PRIVMSG <target> :<msg>) | server send(:<host> PRIVMSG <target> :<msg>)
	else if (!line.front().compare(0, 8,"PRIVMSG ")) {
		line.front() = line.front().substr(line.front().find_first_of(" \r\n") + 1);
		return new PrivMsg();
	}
	// client send(NICK <new nickname>) | server send(:<nickname> NICK :<new nickname>)
	else if (!line.front().compare(0, 5,"NICK ")) {
		line.front() = line.front().substr(line.front().find_first_of(" \r\n") + 1);
		return new Nickname();
	}
	else 
		return NULL;
	// client send(JOIN <channel name>) | server send(:<nickname> JOIN <channel name>)
}

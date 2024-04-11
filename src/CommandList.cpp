#include "../include/CommandList.hpp"
#include "../include/Username.hpp"
#include "../include/Nickname.hpp"
#include "../include/PrivMsg.hpp"
// #include "../include/Invite.hpp"
#include "../include/Topic.hpp"
// #include "../include/Oper.hpp"
// #include "../include/Kick.hpp"
#include "../include/Part.hpp"
#include "../include/Mode.hpp"
#include "../include/Join.hpp"
#include "../include/Ping.hpp"
#include "../include/Pass.hpp"

CommandList::CommandList() {
	_cmdList["PASS"] = new Pass;
	_cmdList["PING"] = new Ping;
	_cmdList["JOIN"] = new Join;
	_cmdList["MODE"] = new Mode;
	_cmdList["PART"] = new Part;
	// _cmdList["KICK"] = new Kick;
	// _cmdList["OPER"] = new Oper;
	_cmdList["TOPIC"] = new Topic;
	// _cmdList["INVITE"] = new Invite;
	_cmdList["NICK"] = new Nickname;
	_cmdList["USER"] = new Username;
	_cmdList["PRIVMSG"] = new PrivMsg;
}
// _cmdList["LIST"] = new List;

CommandList::~CommandList() {
	for (std::map<std::string, Command*>::iterator it = _cmdList.begin(); it != _cmdList.end(); it++)
		delete it->second;
	_cmdList.clear();
}

const std::vector<std::string>& CommandList::getCmd() const {
	return _line;
}

Command* CommandList::createCommand() {
	std::string cmd = parseCmdName(_line.front());
	if (_cmdList.find(cmd) != _cmdList.end())
		return _cmdList[cmd];
	else
		return NULL;
}

std::string CommandList::parseCmdName(const std::string& cmd) {
	if (cmd.empty())
		return cmd;
	size_t pos = cmd.find_first_of(" ");
	if (pos == std::string::npos)
		return cmd;
	else
		return cmd.substr(0, pos);
}

void CommandList::parseBuffer(const std::string& buffer) {
	size_t startPos = 0;
	size_t endPos = 0;

	for (size_t i = 0; i < buffer.size();i++) {
		if ((buffer[i] == '\r' && buffer[i + 1] == '\n')) {
			endPos = i;
			_line.push_back(buffer.substr(startPos, endPos - startPos));
			startPos = endPos + 2;
		}
		// /*testing*/
		// else if (buffer[i] == '\n' && buffer[i - 1] != '\r') {
		// 	endPos = i;
		// 	_line.push_back(buffer.substr(startPos, endPos - startPos));
		// 	startPos = endPos + 1;
		// }
		// /*end testing*/
	}
}

void CommandList::pop() {
	_line.erase(_line.begin());
}

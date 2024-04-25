#include "../include/CommandHandler.hpp"

CommandHandler::CommandHandler() {
	_cmdList["PASS"] = new Pass;
	_cmdList["PING"] = new Ping;
	_cmdList["JOIN"] = new Join;
	_cmdList["MODE"] = new Mode;
	_cmdList["PART"] = new Part;
	_cmdList["LIST"] = new List;
	_cmdList["KICK"] = new Kick;
	_cmdList["TOPIC"] = new Topic;
	_cmdList["NICK"] = new Nickname;
	_cmdList["USER"] = new Username;
	_cmdList["INVITE"] = new Invite;
	_cmdList["PRIVMSG"] = new PrivMsg;
}

CommandHandler::~CommandHandler() {
	for (std::map<std::string, Command*>::iterator it = _cmdList.begin(); it != _cmdList.end(); it++)
		delete it->second;
	_cmdList.clear();
}

const std::vector<std::string>& CommandHandler::getVecCmd() const {
	return _line;
}

Command* CommandHandler::getCmd() {
	return _cmd;
}

void CommandHandler::createCommand() {
	std::string cmd = parseCmdName(_line.front());
	if (_cmdList.find(cmd) != _cmdList.end())
		_cmd =  _cmdList[cmd];
	else
		_cmd = NULL;
}

std::string CommandHandler::parseCmdName(const std::string& cmd) {
	if (cmd.empty())
		return cmd;
	size_t pos = cmd.find_first_of(" ");
	if (pos == std::string::npos)
		return cmd;
	else
		return cmd.substr(0, pos);
}

void CommandHandler::parseBuffer(User& user) {
	size_t startPos = 0;
	size_t endPos = 0;

	for (size_t i = 0; i < user.getMsg().size();i++) {
		if ((user.getMsg()[i] == '\r' && user.getMsg()[i + 1] == '\n')) {
			endPos = i;
			_line.push_back(user.getMsg().substr(startPos, endPos - startPos));
			startPos = endPos + 2;
		}
		// /*testing*/
		// else if (user.getMsg()[i] == '\n' && user.getMsg()[i - 1] != '\r') {
		// 	endPos = i;
		// 	_line.push_back(user.getMsg().substr(startPos, endPos - startPos));
		// 	startPos = endPos + 1;
		// }
		// /*end testing*/
	}
}

void CommandHandler::pop() {
	_line.erase(_line.begin());
}

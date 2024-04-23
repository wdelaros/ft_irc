#pragma once

#include "../include/Username.hpp"
#include "../include/Nickname.hpp"
#include "../include/PrivMsg.hpp"
#include "../include/Invite.hpp"
#include "../include/Topic.hpp"
#include "../include/Kick.hpp"
#include "../include/Part.hpp"
#include "../include/Mode.hpp"
#include "../include/Join.hpp"
#include "../include/Ping.hpp"
#include "../include/Pass.hpp"
#include "../include/List.hpp"

class CommandHandler {
	private:
		std::map<std::string, Command*>	_cmdList;
		std::vector<std::string>		_line;
		Command* _cmd;

	public:
		CommandHandler();
		~CommandHandler();

		const std::vector<std::string>& getVecCmd() const;
		Command* getCmd();

		void createCommand();
		std::string parseCmdName(const std::string& cmd);
		void parseBuffer(const std::string& buffer);
		void pop();
};
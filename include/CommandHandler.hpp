#pragma once

#include "Username.hpp"
#include "Nickname.hpp"
#include "PrivMsg.hpp"
#include "Invite.hpp"
#include "Topic.hpp"
#include "Kick.hpp"
#include "Part.hpp"
#include "Mode.hpp"
#include "Join.hpp"
#include "Ping.hpp"
#include "Pass.hpp"
#include "List.hpp"
#include "User.hpp"

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
		void parseBuffer(User& user);
		void pop();
};
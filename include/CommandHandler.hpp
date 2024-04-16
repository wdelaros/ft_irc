#pragma once

#include "Command.hpp"
#include <vector>

class CommandHandler {
	private:
		std::map<std::string, Command*>	_cmdList;
		std::vector<std::string>		_line;

	public:
		CommandHandler();
		~CommandHandler();

		const std::vector<std::string>& getCmd() const;

		Command* createCommand();
		std::string parseCmdName(const std::string& cmd);
		void parseBuffer(const std::string& buffer);
		void pop();
};
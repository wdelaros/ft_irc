#pragma once

#include "Command.hpp"
#include <vector>

class CommandList {
	private:
		std::map<std::string, Command*>	_cmdList;
		std::vector<std::string>		_line;

	public:
		CommandList();
		~CommandList();

		const std::vector<std::string>& getCmd() const;

		Command* createCommand();
		std::string parseCmdName(const std::string& cmd);
		void parseBuffer(const std::string& buffer);
		void pop();
};
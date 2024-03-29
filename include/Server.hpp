#pragma once

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>

#include "Command.hpp"
#include "CommandList.hpp"
#include "Channel.hpp"
#include "User.hpp"

class Server {
	private:
		int						_userCount;
		std::string				_password;

		std::vector<pollfd>		_poll;
		std::map<int, User*>	_listUser;
		std::map<std::string, Channel*>	_listChannel;

		struct sockaddr_in		_address;

		CommandList				_cmdList;

	public:
		Server(char **argv);
		~Server();

		const int& getFd() const;
		const std::string& getPassword() const;

		void run();

		const std::string Auth(Command *cmd, std::string& buffer, User &eventUser);
		void handleMsg(const std::string& msg, User& eventUser);

		void acceptConnection();
		void userCreation(const int& fd);
		void disconnectUser(int i, int& fd);
		bool nicknameInUse(const std::string& nickname);

		int findNickFd(const std::string& nickname);
};

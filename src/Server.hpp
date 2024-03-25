#pragma once

#include "Command.hpp"
#include "User.hpp"
#include <map>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <poll.h>

class Command;

class Server {
	private:
		int						_userCount;
		std::string				_password;

		std::vector<pollfd>		_poll;
		std::map<int, User*>	_listUser;

		struct sockaddr_in		_address;


	public:
		Server(char **argv);
		~Server();

		const int& getFd() const;
		const std::string& getPassword() const;

		void run();
		void acceptConnection();
		void disconnectUser(int i, int& fd);
		void userCreation(const int& fd);
		bool nicknameInUse(const std::string& nickname);
		const std::string Auth(Command *cmd, std::vector<std::string>& buffer, User &eventUser);
		void handleMsg(const std::string& msg, User& eventUser);
		std::string sendPrivMsg(const std::string& msg, const std::string& nickname);
};

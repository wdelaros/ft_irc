#pragma once

#ifdef __APPLE
# include <cstddef>
# include <sys/_endian.h>
# include <sys/_types/_socklen_t.h>
#endif

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>

#include "Command.hpp"
#include "CommandList.hpp"
#include "User.hpp"

// #include <iostream>
// #include <map>
// #include <netinet/in.h>
// #include <stdexcept>
// #include <string>
// #include <sys/fcntl.h>
// #include <sys/poll.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <vector>

class Server {
	private:
		int						_userCount;
		std::string				_password;

		std::vector<pollfd>		_poll;
		std::map<int, User*>	_listUser;

		struct sockaddr_in		_address;

		CommandList				_cmdList;


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
		const std::string Auth(Command *cmd, std::string& buffer, User &eventUser);
		void handleMsg(const std::string& msg, User& eventUser);
		std::string sendPrivMsg(const std::string& msg, const std::string& nickname);
};

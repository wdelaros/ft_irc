#pragma once

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <netdb.h>
#include <poll.h>

#include "Define.hpp"

#include "CommandHandler.hpp"
#include "Channel.hpp"

class Server {
	private:
		int						_userCount;
		std::string				_password;

		std::vector<pollfd>		_poll;
		std::map<int, User*>	_listUser;
		std::map<std::string, Channel*>	_listChannel;
		// std::map<std::string, std::map<Channel*, bool>>	_listChannel;

		struct sockaddr_in		_address;

		CommandHandler			_cmdHandler;

	public:
		Server(char **argv);
		~Server();

		const int& getFd() const;
		const std::string& getPassword() const;
		const std::map<int, User*>& getUserList() const;
		const std::map<std::string, Channel*>& getChannelList() const;

		void run();

		const std::string Auth(std::string& buffer, User &eventUser);
		void handleMsg(const std::string& msg, User& eventUser);

		/***********************CONNECTION***********************/

		void acceptConnection();
		void userCreation(const int& fd);
		void disconnectUser(int i, int& fd);
		void disconnectUserChannel(User& user);

		/***********************NICKNAME***********************/

		int findNickFd(const std::string& nickname);
		bool nicknameInUse(const std::string& nickname);

		/***********************CHANNEL***********************/

		bool isChannelExist(const std::string& channelName);
		void addChannel(const std::string& channelName, Channel* channel);
		Channel* getChannel(const std::string& channel);
		void deleteChannel(const std::string& channelName);
};

std::vector<std::string> tokenize(const std::string& buffer, const std::string& delimiter);

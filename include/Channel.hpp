#pragma once

#include "User.hpp"
#include <sys/socket.h>
#include <vector>
#include <map>

class User;

class Channel {
	private:
		int			_userCount;
		int			_limitUser;
		std::string	_name;
		std::string	_key;
		std::string	_topic;

		std::vector<User*> _invite;
		std::map<char, bool> _mode;
		std::map<User*, bool> _user;

	public:
		Channel();
		Channel(const std::string& channelName, User* user);
		~Channel();

		const int& getUserCount() const;
		const int& getLimitUser() const;
		const bool& getIsOp(User& user);
		const std::string& getKey() const;
		const std::string& getName() const;
		const std::string& getTopic() const;
		const bool& getMode(char mode);
		std::map<User*, bool>& getUserList();
		std::vector<User*>& getInviteList();

		void setLimitUser(const int& limit);
		void setKey(const std::string& key);
		void setTopic(const std::string& topic);
		void setMode(const char& mode, bool status);

		void sendMsg(const User& user, const std::string& msg);
		void sendUserList(const User* user);
		void sendTopic(const User* user);
		void sendBroadcastUserList();
		void addUser(User* user);

		void sendBroadcastAll(const std::string& msg);
		void disconnectUser(User* user, const std::string& msg);

		bool isUserInChannel(const std::string& nickname);
		bool isUserInInviteList(User* user);
		void sendMode(User& user);
};
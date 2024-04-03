#pragma once

#include <map>
#include "User.hpp"
#include <string>
#include <vector>

class User;

class Channel {
	private:
		int			_userCount;
		int			_limitUser;
		std::string	_name;
		std::string	_key;
		std::string _mode;
		// 

		std::vector<User*> _invite;
		std::map<User*, bool> _user;

	public:
		Channel();
		Channel(const std::string& channelName, User* user);
		~Channel();

		const int& getUserCount() const;
		const int& getLimitUser() const;
		const std::string& getKey() const;
		const std::string& getName() const;
		const std::string& getMode() const;
		std::map<User*, bool>& getUserList();
		std::vector<User*>& getInviteList();

		void setLimitUser(const int& limit);
		void setKey(const std::string& key);
		void setMode(const std::string& mode);

		void sendMsg(const User& user, const std::string& msg);
		void sendUserList(const User* user);
		void sendBroadcastUserList();
		void addUser(User* user);

		void sendBroadcastAll(const std::string& msg);
		void disconnectUser(User* user, const std::string& msg);

		bool isUserInChannel(const std::string& nickname);
		bool isUserInInviteList(User* user);
		void sendMode(User& user);
};
#pragma once

#include <map>
#include "User.hpp"
#include <string>

class User;

class Channel {
	private:
		int			_userCount;
		int			_limitUser;
		std::string	_name;
		std::string	_key;

		std::map<User*, bool> _user;

	public:
		Channel();
		Channel(const std::string& channelName, const std::string& key, User* user);
		~Channel();

		const int& getUserCount() const;
		const int& getLimitUser() const;
		const std::string& getKey() const;

		void setLimitUser();
		void setKey(const std::string& key);

		void sendMsg(const User& user, const std::string& msg);
		void sendUserList(const User* user);
		void sendBroadcastUserList();
		void addUser(User* user);
};
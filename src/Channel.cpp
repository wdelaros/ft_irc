#include "../include/Channel.hpp"
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <utility>

Channel::Channel(const std::string& channelName, const std::string& key, User* user): _name(channelName), _key(key) {
	_user[user] = true;
	_userCount = 1;
}

Channel::~Channel() {

}

const int& Channel::getUserCount() const {
	return _userCount;
}

const int& Channel::getLimitUser() const {
	return _limitUser;
}

const std::string& Channel::getKey() const {
	return _key;
}

const std::string& Channel::getName() const {
	return _name;
}

void Channel::setLimitUser() {

}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::sendMsg(const User& user, const std::string& msg) {
	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++) {
		if (it->first->getFd() != user.getFd())
			send(it->first->getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Channel::sendUserList(const User* user) {
	std::string list = "";
	std::string finalMsg;

	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++) {
		if (it->first->getIsOp())
			list += "+";
		else if (it->second)
			list += "@";
		list += it->first->getNickname() + " ";
	}
	finalMsg = "353 " + user->getNickname() + " = " + _name + " :" + list + "\r\n";
	send(user->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
	finalMsg = "366 " + user->getNickname() + " " + _name + " :End of /NAMES list" + "\r\n";
	send(user->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
}

void Channel::sendBroadcastUserList() {
	std::string list = "";
	std::string finalMsg;

	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++) {
		if (it->first->getIsOp())
			list += "+";
		else if (it->second)
			list += "@";
		list += it->first->getNickname() + " ";
	}
	finalMsg = "353 = " + _name + " :" + list + "\r\n";
	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++) {
		// std::string tmpMsg = FinalMsg + "366 " + it->first->getNickname() + " " + _name + " :End of /NAMES list" + "\r\n";
		send(it->first->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
	}
}

void Channel::addUser(User* user) {
	_user[user] = false;
	_userCount++;
}

void Channel::sendBroadcastAll(const std::string& msg) {
	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++)
		send(it->first->getFd(), msg.c_str(), msg.size(), 0);
}

void Channel::disconnectUser(User* user, const std::string& msg) {
	sendBroadcastAll(":" + user->getNickname() + " PART " + _name + " :" + msg + "\r\n");
	_user.erase(user);
	_userCount--;
}

bool Channel::isUserInChannel(const std::string& nickname) {
	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++)
		if (it->first->getNickname() == nickname)
			return true;
	return false;
}
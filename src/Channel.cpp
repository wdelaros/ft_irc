#include "../include/Channel.hpp"
#include "../include/Define.hpp"
#include <map>

Channel::Channel(const std::string& channelName, User* user): _name(channelName) {
	_user[user] = true;
	_mode['i'] = false;
	_mode['t'] = true;
	_mode['k'] = false;
	_mode['o'] = false;
	_mode['l'] = false;
	_key = "";
	_topic = "";
	_userCount = 1;
	_limitUser = -1;
}

Channel::~Channel() {

}

const int& Channel::getUserCount() const {
	return _userCount;
}

const int& Channel::getLimitUser() const {
	return _limitUser;
}

const bool& Channel::getIsOp(User& user) {
	return _user[&user];
}

const std::string& Channel::getKey() const {
	return _key;
}

const std::string& Channel::getName() const {
	return _name;
}

const std::string& Channel::getTopic() const {
	return _topic;
}

const bool& Channel::getMode(char mode) {
	return _mode[mode];
}

std::map<User*, bool>& Channel::getUserList() {
	return _user;
}

std::vector<User*>& Channel::getInviteList() {
	return _invite;
}

User& Channel::getUser(std::string nickname) {
	std::map<User*, bool>::iterator it = _user.begin();
	for (; it != _user.end(); it++) {
		if (it->first->getNickname() == nickname)
			break ;
	}
	return *it->first;
}

void Channel::setLimitUser(const int& limit) {
	_limitUser = limit;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

void Channel::setMode(const char& mode, bool status) {
	_mode[mode] = status;
}

void Channel::setOp(User &user, bool status) {
	_user[&user] = status;
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
	finalMsg = RPL_NAMREPLY(user->getNickname(), _name, list);
	send(user->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
	finalMsg = RPL_ENDOFNAMES(user->getNickname(), _name);
	send(user->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
}

void Channel::sendTopic(const User* user) {
	std::string finalMsg;

	if (_topic.empty())
		finalMsg = RPL_NOTOPIC(user->getNickname(), _name);
	else
		finalMsg = RPL_TOPIC(user->getNickname(), _name, _topic);
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
	finalMsg = RPL_NAMREPLY(_name, list);
	for (std::map<User*, bool>::iterator it = _user.begin(); it != _user.end(); it++) {
		// std::string tmpMsg = FinalMsg + "366 " + it->first->getNickname() + " " + _name + " :End of /NAMES list" + "\r\n";
		send(it->first->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
	}
}

void Channel::sendMode(User& user) {
	std::string msg;
	std::string mode;

	for (std::map<char, bool>::iterator it = _mode.begin(); it != _mode.end(); it++)
		if (it->second == true)
			mode += it->first;

	if (mode.empty())
		msg = RPL_CHANNELMODEIS(user.getNickname(), _name);
	else
		msg = RPL_CHANNELMODEIS(user.getNickname(), _name, mode);
	send(user.getFd(), msg.c_str(), msg.size(), 0);
}

void Channel::addUser(User* user) {
	_user[user] = false;
	_userCount++;
}

void Channel::addInvite(User* user) {
	for (std::vector<User*>::iterator it = _invite.begin(); it != _invite.end(); it++)
		if ((*it)->getNickname() == user->getNickname())
			return ;
	_invite.push_back(user);
}

void Channel::deleteInvite(User* user) {
	for (std::vector<User*>::iterator it = _invite.begin(); it != _invite.end(); it++) {
		if ((*it)->getNickname() == user->getNickname()) {
			_invite.erase(it);
			break ;
		}
	}
	// for (std::vector<User*>::iterator it = _invite.begin(); it != _invite.end(); it++)
	// 	std::cout << (*it)->getNickname() << std::endl;
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

bool Channel::isUserInInviteList(User* user) {
	for (std::vector<User*>::iterator it = _invite.begin(); it != _invite.end(); it++)
		if ((*it)->getNickname() == user->getNickname())
			return true;
	return false;
}
#include "../include/User.hpp"

User::User() {
	_fd = -1;
	_register[0] = false;
	_register[1] = false;
	_nickname = "user";
	_username = "user";
	_msg = "";
}

User::User(std::string username, std::string nickname, int fd): _fd(fd), _nickname(nickname), _username(username) {
	_register[0] = false;
	_register[1] = false;
	_msg = "";
}

User::~User() {
	close(_fd);
}

const int& User::getFd() const {
	return _fd;
}

const bool& User::getIsAuth() const {
	return _register[0];
}

const bool& User::getHavePass() const {
	return _register[1];
}

const std::string& User::getUsername() const {
	return _username;
}

const std::string& User::getNickname() const {
	return _nickname;
}

const std::string& User::getMsg() const {
	return _msg;
}

void User::setFd(const int& fd) {
	_fd = fd;
}

void User::setIsAuth(const bool& auth) {
	_register[0] = auth;
}

void User::setHavePass(const bool& pass) {
	_register[1] = pass;
}

void User::setUsername(const std::string& username) {
	if (username != "user")
		_username = username;
}

void User::setNickname(const std::string& nickname) {
	if (nickname != "user")
		_nickname = nickname;
}

void User::setMsg(const std::string& msg) {
		_msg += msg;
}

void User::clearMsg() {
	_msg = "";
}

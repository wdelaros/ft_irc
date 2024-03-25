#include "Server.hpp"
#include "Command.hpp"
#include "User.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#ifdef __linux
# include <endian.h>
# include <sys/socket.h>
# include <asm-generic/errno.h>
#else
# include <sys/_endian.h>
# include <sys/_types/_socklen_t.h>
#endif

extern bool g_stop;

Server::Server(char **argv): _userCount(0) {
	struct pollfd listpoll;
	listpoll.events = POLLIN;
	listpoll.fd = -1;
	_poll.push_back(listpoll);


	int fd;
	std::string port = argv[1];
	_password = argv[2];
	if (port.empty())
		throw std::invalid_argument("Error: port empty!");
	else if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error: port invalid!");

	if (_password.empty())
		throw std::invalid_argument("Error: empty password!");

	_address.sin_port = htons(atoi(port.c_str()));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	fd = socket(_address.sin_family, SOCK_STREAM, 0);
	if (fd == -1)
		throw std::runtime_error(strerror(errno));
	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &l, sizeof(l)) == -1)
		throw std::runtime_error(strerror(errno));
	fcntl(fd, F_SETFL, O_NONBLOCK);
	if (bind(fd, (struct sockaddr*)&_address, sizeof(_address)) == -1) {
		close(fd);
		throw std::runtime_error(strerror(errno));
	}
	if (listen(fd, SOMAXCONN) == -1) {
		close(fd);
		throw std::runtime_error(strerror(errno));
	}
	_poll[0].fd = fd;
}

Server::~Server() {
	std::map<int, User *>::iterator it = _listUser.begin();
	while (it != _listUser.end()) {
		delete it->second;
		it++;
	}
	close(_poll[0].fd);
}

const int& Server::getFd() const {
	return _poll[0].fd;
}

const std::string& Server::getPassword() const {
	return _password;
}

void	Server::userCreation(const int& fd) {
	struct pollfd	user;
	user.fd = fd;
	user.events = POLLIN;
	user.revents = 0;
	_poll.push_back(user);

	User *userData = new User("", "", fd);
	_listUser[fd] = userData;
	_userCount++;
}

void Server::acceptConnection() {
	socklen_t addrLen = sizeof(_address);
	int user = accept(_poll[0].fd, (sockaddr*)&_address, &addrLen);
	if (user == -1)
		return ;
	std::cout << "Connection accepted fd: " << user  << " ID: " << (user - 3) << std::endl;
	userCreation(user);
}

void Server::disconnectUser(int i, int& fd) {
	delete _listUser[fd];
	_poll.erase(_poll.begin() + i);
	_listUser.erase(fd);
	_userCount--;
}

void Server::run() {
	int fd;
	char buffer[BUFSIZ * 8];
	while (!g_stop) {
		if (poll(_poll.data(), _userCount + 1, 0) == -1)
			throw std::invalid_argument(strerror(errno));
		if (_poll[0].revents & POLLIN)
			acceptConnection();

		for (int i = 1; i <= _userCount; i++) {
			fd = _poll[i].fd;
			if (this->_poll[i].revents & (POLLHUP | POLLERR | POLLNVAL)){
				std::cout << "Client " << this->_listUser[fd]->getNickname() << " " << _poll[i].fd << " disconnected" << std::endl;
				disconnectUser(i, fd);
			}
			else if(_poll[i].revents & POLLIN) {
				User& eventUser = *_listUser[fd];
				int ret = recv(_poll[i].fd, buffer, BUFSIZ * 8, MSG_DONTWAIT);
				if (ret <= 0) {
					disconnectUser(i, fd);
					continue ;
				}
				buffer[ret] = '\0';
				handleMsg(buffer, eventUser);
			}
		}
	}
}

const std::string Server::Auth(Command *cmd, std::vector<std::string>& buffer, User &eventUser) {
	std::string msg;

	if (!eventUser.getHavePass()) {
		if (!buffer.front().compare(0, 5, "PASS ")) {
			if (buffer.front() == "PASS " + _password)
				eventUser.setHavePass(true);
			else
				msg = "451 PRIVMSG :Wrong password, use /set irc.server.<server_name>.password <password>!\r\n";
		}
		else {
			msg = "451 PRIVMSG :Password required! Use /set irc.server.<server_name>.password <password>!\r\n";
			while (buffer.size() > 1)
				buffer.pop_back();
		}
	}
	else if (cmd)
		msg = cmd->execute(*this, eventUser, buffer.front());
	else if (!buffer.front().compare(0, 5, "USER ")) {
		buffer.front() = buffer.front().substr(5);
		eventUser.setUsername(buffer.front().substr(0, buffer.front().find_first_of(" \r\n")));
		if (eventUser.getUsername().empty())
			msg = "451 PRIVMSG :You are not registered. Give a username (/set irc.server.<server name>.username <username>).\r\n";
	}
	else if (!buffer.front().compare(0, 10, "CAP LS 302"))
		;
	else
		msg = "451 PRIVMSG :You are not registered.\r\n";

	if (!eventUser.getNickname().empty() && !eventUser.getUsername().empty() && eventUser.getHavePass() && !eventUser.getIsAuth()) {
		eventUser.setIsAuth(true);
		msg = "001 " + eventUser.getNickname() + " :You are now register. Welcome on ft_irc " + eventUser.getUsername() + "!\r\n";
	}
	return msg;
}

std::vector<std::string> parseBuffer(std::string buffer) {
	std::vector<std::string> line;
	size_t startPos = 0;
	size_t endPos = 0;

	for (size_t i = 0; i < buffer.size();i++) {
		if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
			endPos = i;
			line.push_back(buffer.substr(startPos, endPos - startPos));
			startPos = endPos + 2;
		}
	}
	return line;
}

std::string Server::sendPrivMsg(const std::string& msg, const std::string& nickname) {
	for (int i = 1; i <= _userCount; i++) {
		int fd = _poll[i].fd;
		if (_listUser[fd]->getNickname() == nickname) {
			send(fd, msg.c_str(), msg.size(), 0);
			return ("");
		}
	}
	return ("401 PRIVMSG " + nickname + " not found!\r\n");
}

bool Server::nicknameInUse(const std::string& nickname) {
	for (int i = 1; i < _userCount; i++) {
		int fd = _poll[i].fd;
		if (_listUser[fd]->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::handleMsg(const std::string& buffer, User& eventUser) {
	std::string finalMsg;
	std::vector<std::string> line;

	std::cout << "Client " << eventUser.getNickname() << " " << eventUser.getFd() << " sends: " << buffer;

	line = parseBuffer(buffer);
	while (!line.empty()) {
		Command *cmd = NULL;
		cmd = cmd->createCommand(line);
		if (!eventUser.getIsAuth())
			finalMsg = Auth(cmd, line , eventUser);
		else if (cmd)
			finalMsg = cmd->execute(*this, eventUser, line.front());
		if (!finalMsg.empty())
			send(eventUser.getFd(), finalMsg.c_str(), finalMsg.size(), 0);
		line.erase(line.begin());
	}
}

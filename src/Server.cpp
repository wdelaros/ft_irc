#include "../include/Server.hpp"

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
	std::map<std::string, Channel *>::iterator itChannel = _listChannel.begin();
	while (itChannel != _listChannel.end()) {
		delete itChannel->second;
		itChannel++;
	}
	std::map<int, User *>::iterator it = _listUser.begin();
	while (it != _listUser.end()) {
		delete it->second;
		close(it->first);
		it++;
	}
	close(_poll[0].fd);
}

/***********************GETTER***********************/

const int& Server::getFd() const {
	return _poll[0].fd;
}

const std::string& Server::getPassword() const {
	return _password;
}

const std::map<int, User*>& Server::getUserList() const {
	return _listUser;
}

const std::map<std::string, Channel*>& Server::getChannelList() const {
	return _listChannel;
}

/***********************CONNECTION***********************/

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
	disconnectUserFromAllChannel(*_listUser[fd]);
	for (std::map<std::string, Channel*>::iterator it = _listChannel.begin(); it != _listChannel.end(); it++)
		it->second->deleteInvite(_listUser[fd]);
	delete _listUser[fd];
	_poll.erase(_poll.begin() + i);
	_listUser.erase(fd);
	_userCount--;
}

void Server::disconnectUserFromAllChannel(User& user) { // from all channel
	for (std::map<std::string, Channel*>::iterator it = _listChannel.begin(); it != _listChannel.end(); ) {
		if (it->second->isUserInChannel(user.getNickname())) {
			it->second->disconnectUser(&user, "has disconnected!");
			std::cout << "Channel: " << it->first << "	user connected: " << it->second->getUserCount() << std::endl;
			if (!it->second->getUserCount()) {
				deleteChannel(it->second->getName());
				it = _listChannel.begin();
			}
			else
				it++;
		}
		else
			it++;
	}
}

void Server::deleteChannel(const std::string& channelName) {
	Channel* channel = _listChannel[channelName];
	_listChannel.erase(channelName);
	delete channel;
}

/***********************SERVER***********************/

void Server::run() {
	int fd;
	char buffer[8192];
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
				int ret = recv(_poll[i].fd, buffer, 8192, MSG_DONTWAIT);
				if (ret <= 0) {
					std::cout << "Client " << this->_listUser[fd]->getNickname() << " " << _poll[i].fd << " disconnected" << std::endl;
					disconnectUser(i, fd);
					continue ;
				}
				buffer[ret] = '\0';
				handleMsg(buffer, eventUser);
			}
		}
	}
}

const std::string Server::Auth(std::string& buffer, User &eventUser) {
	std::string msg;

	if (!eventUser.getHavePass()) {
		if (!buffer.compare(0, 5, "PASS "))
			msg = _cmdHandler.getCmd()->execute(*this, eventUser, buffer);
		else
			msg = ERR_NOTREGISTERED((std::string)"Password required! Use /set irc.server.<server_name>.password <password>!");
	}
	else if (_cmdHandler.getCmd()->getName() == "nick" || _cmdHandler.getCmd()->getName() == "user")
		msg = _cmdHandler.getCmd()->execute(*this, eventUser, buffer);
	else
		msg = ERR_NOTREGISTERED((std::string)"You are not registered.");

	if (!eventUser.getNickname().empty() && !eventUser.getUsername().empty() && eventUser.getHavePass() && !eventUser.getIsAuth()) {
		eventUser.setIsAuth(true);
		msg = RPL_WELCOME(eventUser.getNickname(), eventUser.getUsername());
	}
	return msg;
}

void Server::handleMsg(const std::string& buffer, User& eventUser) {
	std::string finalMsg;
	std::vector<std::string> vecCmd;

	std::cout << eventUser.getNickname() << " "<< eventUser.getFd() << " :: " << buffer;

	_cmdHandler.parseBuffer(buffer);
	vecCmd = _cmdHandler.getVecCmd();
	for (std::vector<std::string>::iterator it = vecCmd.begin(); it != vecCmd.end(); it++) {
		_cmdHandler.createCommand();
		if (_cmdHandler.getCmd()) {
			if (!eventUser.getIsAuth())
				finalMsg = Auth(*it, eventUser);
			else
				finalMsg = _cmdHandler.getCmd()->execute(*this, eventUser, *it);
		}
		else if (it->compare(0, 10, "CAP LS 302"))
			finalMsg = ERR_UNKNOWNCOMMAND(it->substr(0, it->find(" ")));

		if (!finalMsg.empty())
			send(eventUser.getFd(), finalMsg.c_str(), finalMsg.size(), 0);
		_cmdHandler.pop();
	}
}

std::vector<std::string> tokenize(const std::string& buffer, const std::string& delimiter) {
	std::vector<std::string> vector;
	std::string token;
	size_t startPos = 0;
	size_t endPos = buffer.find(delimiter);

	while (endPos != std::string::npos) {
		token = buffer.substr(startPos, endPos - startPos);
		if (token.find(":") != std::string::npos)
			break ;
		if (token.find(delimiter) == std::string::npos)
			vector.push_back(token);
		startPos = endPos + 1;
		endPos = buffer.find(delimiter, startPos);
	}
	token = buffer.substr(startPos);
	if (token.find(delimiter) == std::string::npos || token.find(":") != std::string::npos)
		vector.push_back(token);
	return vector;
}

/***********************NICKNAME***********************/

bool Server::nickIsAuth(const std::string& nickname) {
	int fd = findNickFd(nickname);
	if (fd != -1)
		if (_listUser[fd]->getIsAuth())
			return true;
	return false;
}

int Server::findNickFd(const std::string& nickname) {
	int fd;
	if (!nicknameInUse(nickname))
		return -1;
	for (int i = 1; i <= _userCount; i++) {
		fd = _poll[i].fd;
		if (_listUser[fd]->getNickname() == nickname)
			break ;
	}
	return fd;
}

bool Server::nicknameInUse(const std::string& nickname) {
	for (int i = 1; i <= _userCount; i++) {
		int fd = _poll[i].fd;
		if (!strcasecmp(_listUser[fd]->getNickname().c_str(), nickname.c_str()))
			return true;
	}
	return false;
}

/***********************CHANNEL***********************/

bool Server::isChannelExist(const std::string& channelName) {
	std::map<std::string, Channel*>::iterator it = _listChannel.find(channelName);
	return it != _listChannel.end();
}

void Server::addChannel(const std::string& channelName, Channel* channel) {
	_listChannel[channelName] = channel;
}

Channel* Server::getChannel(const std::string& channel) {
	return _listChannel[channel];
}

#include "IRC.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

IRC::IRC(const uint16_t port, const std::string password): _password(password) {
	_fd = _getSocket(port);
	_run();
}

IRC::~IRC() {

}

const int& IRC::getFd() const {
	return _fd;
}

void	IRC::_newUser(const int& fd) {
	pollfd	user;
	user.fd = fd;
	user.events = POLLIN;
	user.revents = 0;

	t_user userData;
	userData.fd = fd;
	userData.username = "";
	userData.nickname = "";

	_users[fd] = userData;
	_poll.push_back(user);
}

void IRC::_acceptConnection() {
	sockaddr_in     clientAddr;
	socklen_t       clientInfo = sizeof(clientAddr);
	int user = accept(_fd, (sockaddr*)&clientAddr, &clientInfo);

	if  (user != -1)
		_newUser(user);
}

void IRC::_run() {
	_newUser(_fd);

	int res;
	while (_poll.size() > 0) {
		res = poll(_poll.data(), _poll.size(), -1);

		if (_poll[0].revents & POLLIN) {
			_acceptConnection();
			std::cout << "client: " << res << std::endl;
			res--;
		}
	}
}

int IRC::_getSocket(uint16_t port) {
	int fd;
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	if (getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result))
		throw std::invalid_argument(strerror(errno));

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd == -1)
			continue ;
		if (!bind(fd, rp->ai_addr, rp->ai_addrlen))
			break ;
		close(fd);
	}
	freeaddrinfo(result);
	if (rp == NULL)
		throw std::invalid_argument(strerror(errno));

	if (listen(fd, SOMAXCONN) < 0)
		throw std::invalid_argument(strerror(errno));

	return fd;
}

// int connection = accept(fd, hints.ai_addr, &hints.ai_addrlen);
// if (connection < 0)
// 	throw std::invalid_argument(strerror(errno));
// char buffer[1024];
// recv(connection, buffer, 1024, MSG_WAITALL);
// std::cout << buffer << std::endl;
// close(connection);
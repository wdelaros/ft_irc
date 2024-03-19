#pragma once

#include <map>
#include <string>
#include <vector>
#include <poll.h>

typedef struct	s_user {
	int	fd;
	std::string	username;
	std::string	nickname;
}				t_user;

class IRC {
	private:
		std::string	_password;
		int			_fd;

		std::vector<pollfd>		_poll;
		std::map<int, t_user>	_users;

		int	_getSocket(uint16_t port);
		void _newUser(const int& fd);
		void _acceptConnection();
		void _run();

	public:
		IRC(const uint16_t port, const std::string password);
		~IRC();

		const int& getFd() const;

};
#pragma once

#include <string>
class User {
	private:
		int _fd;
		bool _register[2]; // [0] Authentification	[1] Password
		std::string _msg;
		std::string _nickname;
		std::string _username;

	public:
		User();
		User(std::string username, std::string nickname, int fd);
		~User();

		const int& getFd() const;
		const bool& getIsAuth() const;
		const bool& getHavePass() const;
		const std::string& getMsg() const;
		const std::string& getUsername() const;
		const std::string& getNickname() const;

		void setFd(const int& fd);
		void setIsAuth(const bool& auth);
		void setHavePass(const bool& pass);
		void setMsg(const std::string& msg);
		void setUsername(const std::string& username);
		void setNickname(const std::string& nickname);

};
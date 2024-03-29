#pragma once

#include <string>

class User;

class Channel {
	private:
		int			_userCount;
		int			_limitUser;
		std::string	_password;

	public:
		Channel();
		~Channel();

		const int& getUserCount() const;
		const int& getLimitUser() const;
		const std::string& getPassword() const;

		void setLimitUser();
		void setPassword();
};

// operator <@>
// regular channel userlist:	@nick
// channel doesn't support mode	<+>
// channel <+> don't have operator
// chanenl #
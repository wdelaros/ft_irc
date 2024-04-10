#pragma once

#include "User.hpp"
#include <iostream>
#include <map>
#include <string>

class Server;

class Command {
	public:
		Command();
		virtual ~Command();

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const = 0;
};
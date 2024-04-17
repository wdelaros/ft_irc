#pragma once

#include "User.hpp"
#include <iostream>
#include <vector>
#include <map>

class Server;

class Command {
	public:
		Command();
		virtual ~Command();

		virtual const std::string& getName() const = 0;

		virtual std::string execute(Server& server, User& eventUser, std::string& buffer) const = 0;
};
#include "../include/Oper.hpp"
#include "../include/Server.hpp"

Oper::Oper(): _name("oper") {

}

Oper::~Oper() {

}

const std::string& Oper::getName() const {
	return _name;
}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Oper::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	
	return msg;
}
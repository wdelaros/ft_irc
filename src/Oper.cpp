#include "../include/Oper.hpp"
#include "../include/Server.hpp"

Oper::Oper() {

}

Oper::~Oper() {

}

// client send(MODE <channel> (+,-)<mode param> <nickname>) | server send(:<nickname> MODE <channel> (+,-)<mode param>)
std::string Oper::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	
	return msg;
}
#include "../include/Pass.hpp"
#include "../include/Server.hpp"

Pass::Pass(): _name("pass") {

}

Pass::~Pass() {

}

const std::string& Pass::getName() const {
	return _name;
}

std::string Pass::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);

	if (eventUser.getIsAuth())
		return ERR_ALREADYREGISTRED(eventUser.getNickname());
	else if (eventUser.getHavePass())
		return ERR_UNKNOWNERROR(eventUser.getNickname(), buffer, "Password already given");
	if (buffer == "PASS " + server.getPassword()) {
		eventUser.setHavePass(true);
		std::cout << "Password accepted!" << std::endl;
	}
	else
		msg = ERR_NOTREGISTERED((std::string)"Wrong password, use /set irc.server.<server_name>.password <password>!");
	return msg;
}
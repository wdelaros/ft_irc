#include "../include/Mode.hpp"
#include "../include/Server.hpp"
#include <cstddef>
#include <vector>

Mode::Mode(): _name("mode") {

}

Mode::~Mode() {

}

const std::string& Mode::getName() const {
	return _name;
}

int checkMode(const std::string& mode, const std::string& delimiter) {
	if (mode[0] != '+' && mode[0] != '-')
		return false;
	for (size_t i = 1; i < mode.size(); i++) {
		if ((mode[i - 1] == '-' || mode[i - 1] == '+') && (mode[i] == '+' || mode[i] == '-'))
			return false;
		if (delimiter.find(mode[i]) == std::string::npos && mode[i] != '+' && mode[i] != '-')
			return false;
	}
	return true;
}

std::string parseMode(Channel* channel, const std::string& mode) {
	std::string msg;

	if (channel->getMode().find("k") != std::string::npos && mode.find("k") != std::string::npos) {
		if (mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = ERR_KEYSET(channel->getName());
	}
	if (channel->getMode().find("k") != std::string::npos && mode.find("k") != std::string::npos) {
		if (mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = ERR_KEYSET(channel->getName());
	}
	if (channel->getMode().find("k") != std::string::npos && mode.find("k") != std::string::npos) {
		if (mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = ERR_KEYSET(channel->getName());
	}
	if (channel->getMode().find("k") != std::string::npos && mode.find("k") != std::string::npos) {
		if (mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = ERR_KEYSET(channel->getName());
	}
	return msg;
}

// client send(MODE <channel> (+,-)<mode> <mode params>) | server send(:<nickname> MODE <channel> (+,-)<mode> <mode params>)
std::string Mode::execute(Server& server, User& eventUser, std::string& buffer) const {
	std::string msg;
	std::vector<std::string> vec = tokenize(buffer, " ");

	if (vec.size() < 2)
		return ERR_NEEDMOREPARAMS(eventUser.getNickname(), vec[0]);

	if (server.isChannelExist(vec[1])) {
		Channel* channel = server.getChannel(vec[1]);
		if (channel->isUserInChannel(eventUser.getNickname())) {
			if (vec.size() > 2) {
				if (channel->getUserList()[&eventUser]) {
					if (checkMode(vec[2], "itkol"))
						msg = parseMode(channel, vec[2]);
					else
						msg = ERR_UNKNOWNMODE(vec[2], vec[1]);
				}
				else
					msg = ERR_CHANOPRIVSNEEDED(vec[1]);
			}
			else
				channel->sendMode(eventUser);
		}
		else
			msg = ERR_USERNOTINCHANNEL(eventUser.getNickname(), channel->getName());
	}
	else
		msg = ERR_NOSUCHCHANNEL(vec[1]);
	return msg;
}

// i	invitation				MODE (+,-)i
// t	restiction topic op		MODE (+,-)t
// k	password(key)			MODE (+,-)k <key>
// o	privilege op			MODE (+,-)o <nickname>
// l	limit user				MODE (+l <limit>,-l)


//call Invite cmd or add to invite list in channel
std::string invitationMode(Channel* chan, User& eventUser, char modif)
{
	std::string msg;
	if (chan->findMode('i') && modif == '-')
	{
		chan->setMode('i', false);
		msg = ":" + eventUser.getNickname() + " removed invitation only mode"; // WIP
	}
	else if (!chan->findMode('i') && modif == '+')
	{
		chan->setMode('i', true);
		msg = ":" + eventUser.getNickname() + " added invitation only mode"; // WIP
	}
	else
		msg = "Error: Channel mode cannot be change to current mode"; // WIP
	return (msg);
}

//(todo: add Operator toggle on topic in channels) toggles topic for ops whenever called
std::string restrictionTopicMode(Channel* chan, User& eventUser, char modif)
{
	std::string msg;
	if (chan->findMode('t') && modif == '-')
	{
		chan->setMode('t', false);
		msg = ":" + eventUser.getNickname() + " removed invitation only mode"; // WIP
	}
	else if (!chan->findMode('t') && modif == '+')
	{
		chan->setMode('t', true);
		msg = ":" + eventUser.getNickname() + " added invitation only mode"; // WIP
	}
	else
		msg = "Error: Channel mode cannot be change to current mode"; // WIP
	return (msg);
}

//adds password to channel, maybe changes if already has password
std::string passwordMode(Channel* chan, User& eventUser, char modif, std::string passw)
{
	std::string msg;
	if (chan->findMode('k') && modif == '-')
	{
		chan->setMode('k', false);
		msg = ":" + eventUser.getNickname() + " removed invitation only mode"; // WIP
	}
	else if (!chan->findMode('k') && modif == '+')
	{
		chan->setMode('k', true);
		msg = ":" + eventUser.getNickname() + " added invitation only mode"; // WIP
	}
	else
		msg = "Error: Channel mode cannot be change to current mode"; // WIP
	return (msg);
}

//modifies op privilege to target user
std::string privilegeMode(Channel* chan, User& eventUser, char modif, std::string targetName)
{
	std::string msg;
	if (chan->findMode('o') && modif == '-')
	{
		chan->setMode('o', false);
		msg = ":" + eventUser.getNickname() + " removed invitation only mode"; // WIP
	}
	else if (!chan->findMode('o') && modif == '+')
	{
		chan->setMode('o', true);
		msg = ":" + eventUser.getNickname() + " added invitation only mode"; // WIP
	}
	else
		msg = "Error: Channel mode cannot be change to current mode"; // WIP
	(void)targetName;
	return (msg);
}

//modifies limit amount of users for channel
std::string limitMode(Channel* chan, User& eventUser, char modif, int limit)
{
	std::string msg;
	if (chan->findMode('l') && modif == '-')
	{
		chan->setMode('l', false);
		msg = ":" + eventUser.getNickname() + " removed invitation only mode"; // WIP
	}
	else if (!chan->findMode('l') && modif == '+')
	{
		chan->setMode('l', true);
		msg = ":" + eventUser.getNickname() + " added invitation only mode"; // WIP
	}
	else
		msg = "Error: Channel mode cannot be change to current mode"; // WIP
	return (msg);
}

// oper <nickname> <password>
// IRC Operator = god
// Channel Operator = jesus

// IRC Operator can force change channel mode

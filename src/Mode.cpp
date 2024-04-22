#include "../include/Mode.hpp"
#include "../include/Server.hpp"
#include <string>

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

// i	invitation				MODE (+,-)i
// t	restiction topic op		MODE (+,-)t
// k	password(key)			MODE (+,-)k <key>
// o	privilege op			MODE (+,-)o <nickname>
// l	limit user				MODE (+l <limit>,-l)


//call Invite cmd or add to invite list in channel
std::string Mode::invitationMode(Channel* chan, User& eventUser, char modif) const
{
	std::string msg;
	if (chan->getMode('i') && modif == '-')
	{
		chan->setMode('i', false);
		msg = MODE(eventUser.getNickname(), chan->getName(), modif, 'i', "");
	}
	else if (!chan->getMode('i') && modif == '+')
	{
		chan->setMode('i', true);
		msg = MODE(eventUser.getNickname(), chan->getName(), modif, 'i', "");
	}
	else
		return (ERR_UNKNOWNERROR(chan->getName(), "MODE", "Invite is already set to " + modif + "i"));
	chan->sendBroadcastAll(msg);
	return ("");
}

//(todo: add Operator toggle on topic in channels) toggles topic for ops whenever called
std::string Mode::restrictionTopicMode(Channel* chan, User& eventUser, char modif) const
{
	std::string msg;
	if (chan->getMode('t') && modif == '-')
	{
		chan->setMode('t', false);
		msg = MODE(eventUser.getNickname(), chan->getName(), modif, 't', "");
	}
	else if (!chan->getMode('t') && modif == '+')
	{
		chan->setMode('t', true);
		msg = MODE(eventUser.getNickname(), chan->getName(), modif, 't', "");
	}
	else
		return (ERR_UNKNOWNERROR(chan->getName(), "MODE", "Topic is already set to " + modif + "t"));
	chan->sendBroadcastAll(msg);
	return ("");
}

//adds password to channel, maybe changes if already has password
std::string Mode::passwordMode(Channel* chan, User& eventUser, char modif, std::string passw) const
{
	std::string msg;
	if (chan->getMode('k') && modif == '-')
	{
		if (chan->getKey() == passw)
		{
			chan->setMode('k', false);
			chan->setKey("");
		}
		else
			return (ERR_UNKNOWNERROR(eventUser.getNickname(), "MODE", "Given key does not match current key"));
	}
	else if (!chan->getMode('k') && modif == '+')
	{
		chan->setMode('k', true);
		chan->setKey(passw);
	}
	else
		return (ERR_UNKNOWNERROR(chan->getName(), "MODE", "Channel already has no key"));
	msg = MODE(eventUser.getNickname(), chan->getName(), modif, 'k', "");
	chan->sendBroadcastAll(msg);
	return ("");
}

//modifies op privilege to target user
std::string Mode::privilegeMode(Channel* chan, User& eventUser, char modif, User& target) const
{
	std::string msg;
	if (modif == '-')
		chan->setOp(target, false);
	else if (modif == '+')
		chan->setOp(target, true);
	msg = MODE(eventUser.getNickname(), chan->getName(), modif, 'o', target.getNickname());
	chan->sendBroadcastAll(msg);
	return ("");
}

//modifies limit amount of users for channel
std::string Mode::limitMode(Channel* chan, User& eventUser, char modif, int limit) const
{
	std::string msg;
	if (modif == '-')
		chan->setMode('l', false);
	else if (chan->getUserCount() > limit)
		return (ERR_UNKNOWNERROR(eventUser.getNickname(), "MODE", "Number of users in channel exceeds the limit set"));
	else if (modif == '+')
		chan->setMode('l', true);
	chan->setLimitUser(limit);
	msg = MODE(eventUser.getNickname(), chan->getName(), modif, 'l', std::to_string(limit));
	chan->sendBroadcastAll(msg);
	return ("");
}

std::string Mode::parseMode(Channel* channel, User& eventUser, const std::string& mode, std::string arg) const {
	std::string msg;

	if (mode.find("i") != std::string::npos)
			return (invitationMode(channel, eventUser, mode[0]));
	else if (mode.find("t") != std::string::npos)
			return (restrictionTopicMode(channel, eventUser, mode[0]));
	if (mode.find("k") != std::string::npos) {
		if (channel->getMode('k') && mode.substr(0, mode.find("k")).find_last_of("+") != std::string::npos)
			msg = ERR_KEYSET(channel->getName());
		else
			return (passwordMode(channel, eventUser, mode[0], arg));
	}
	if (mode.find("o") != std::string::npos) {
		if (channel->isUserInChannel(arg))
			return (privilegeMode(channel, eventUser, mode[0], channel->getUser(arg)));
		else
			msg = ERR_USERNOTINCHANNEL(eventUser.getNickname(), channel->getName(), arg);
	}
	if (mode.find("l") != std::string::npos) {
		try
		{
			return (limitMode(channel, eventUser, mode[0], stoi(arg)));
		}
		catch (std::exception& e)
		{
			msg = ERR_UNKNOWNERROR(eventUser.getNickname(), "MODE", "+l invalid argument: not a number");
		}
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
					{
						if (vec.size() == 3 && checkMode(vec[2], "til"))
						{
							if (checkMode(vec[2], "it"))
								msg = parseMode(channel, eventUser, vec[2], "");
							else if (vec[2] == "-l")
								msg = parseMode(channel, eventUser, vec[2], "-1");
							else
								msg = ERR_UNKNOWNERROR(eventUser.getNickname(), "MODE", "Invalid amount of argument");
						}
						else if (vec.size() == 4 && checkMode(vec[2], "kol"))
							msg = parseMode(channel, eventUser, vec[2], vec[3]);
						else
							msg = ERR_UNKNOWNERROR(eventUser.getNickname(), "MODE", "Invalid amount of argument");
					}
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
			msg = ERR_NOTONCHANNEL(eventUser.getNickname(), channel->getName());
	}
	else
		msg = ERR_NOSUCHCHANNEL(vec[1]);
	return msg;
}


// oper <nickname> <password>
// IRC Operator = god
// Channel Operator = jesus

// IRC Operator can force change channel mode

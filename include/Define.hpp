#pragma once

#define GET_MACRO(_1,_2,_3,NAME,...) NAME

#define MODE(nickname, channel, modif, mode, param) ":" + nickname + " MODE " + channel + " " + modif + mode + " " + param + "\r\n";

/* --- Macros --- */
#define RPL_CHANNELMODEIS1(nickname, channel) "324 " + nickname + " " + channel + "\r\n"
#define RPL_CHANNELMODEIS2(nickname, channel, mode) "324 " + nickname + " " + channel + " +" + mode + "\r\n"
#define RPL_NAMREPLY1(channel, list) "353 = " + channel + " :" + list + "\r\n"
#define RPL_NAMREPLY2(nickname, channel, list) "353 " + nickname + " = " + channel + " :" + list + "\r\n"
#define ERR_USERONCHANNEL1(nickname, channel)  "443 " + nickname + " " + channel + " :You're already on that channel!\r\n"
#define ERR_USERONCHANNEL2(nickname, channel, target)  "443 " + nickname + " " + channel + " " + target + " :is already on that channel!\r\n"


/* --- Connection --- */
#define RPL_WELCOME(nickname, username) "001 " + nickname + " You are now registered. Welcome on ft_irc " + username + "!\r\n"


/* --- Command --- */
#define RPL_LIST(nickname, channel, size, topic) "322 " + nickname + " " + channel + " " +  size + " :" + topic + "\r\n"
#define RPL_LISTEND(nickname) "323 " + nickname + " :End of /LIST\r\n"
#define RPL_CHANNELMODEIS(...) /*324*/ GET_MACRO(__VA_ARGS__, RPL_CHANNELMODEIS2, RPL_CHANNELMODEIS1)(__VA_ARGS__)
#define RPL_NOTOPIC(nickname, channel) "331 " + nickname + " " + channel + " :No topic set" + "\r\n"
#define RPL_TOPIC(nickname, channel, topic)  "332 " + nickname + " " + channel + " :" + topic + "\r\n"
#define RPL_INVITING(nickname, target, channel) "341 " + nickname + " " + target + " " + channel + "\r\n"
#define RPL_NAMREPLY(...) /*353*/ GET_MACRO(__VA_ARGS__, RPL_NAMREPLY2, RPL_NAMREPLY1)(__VA_ARGS__)
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + " :End of /NAMES list" + "\r\n"


/* --- Error --- */
#define ERR_UNKNOWNERROR(nickname, command, reason) "400 " + nickname + " " + command + " :" + reason + "\r\n"
#define ERR_NOSUCHNICK(nickname) "401 '" + nickname + "' :not found!\r\n";
#define ERR_NOSUCHCHANNEL(channel) "403 '" + channel + "' :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nickname, channel) "404 " + nickname + " " + channel + " :Cannot send to channel\r\n"
#define ERR_NOORIGIN(nickname) "409 " + nickname + " :No origin specified\r\n"
#define ERR_NORECIPIENT(nickname, command) "411 " + nickname + " :No recipient given (" + command + ")\r\n"
#define ERR_NOTEXTTOSEND(nickname, reason) "412 " + nickname + " :" + reason + "\r\n"
#define ERR_UNKNOWNCOMMAND(command) "421 '" + command + "' :Unknow command!\r\n"
#define ERR_NONICKNAMEGIVEN(nickname) "431 " + nickname + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nickname, reason) "432 '" + buffer + "' :" + reason + "\r\n"
#define ERR_NICKNAMEINUSE(nickname) "433 '" + nickname + "' :Nickname is already in use!\r\n"
#define ERR_USERNOTINCHANNEL(nickname, channel, target) "441 " + nickname + " " + channel + " :" + target + " is not on that channel!\r\n"
#define ERR_NOTONCHANNEL(nickname, channel) "442 " + nickname + " " + channel + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(...) /*443*/ GET_MACRO(__VA_ARGS__, ERR_USERONCHANNEL2, ERR_USERONCHANNEL1)(__VA_ARGS__)
#define ERR_NOTREGISTERED(reason) "451 PRIVMSG " + reason + "\r\n"
#define ERR_NEEDMOREPARAMS(nickname, command) "461 " + nickname + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(nickname) "462 " + nickname + " :Unauthorized command (already registered)\r\n"
#define ERR_KEYSET(channel) "467 " + channel + " :Channel key is already set\r\n"
#define ERR_CHANNELISFULL(channel) "471 '" + channel + "':Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(mode, channel) "472 " + mode + " :is unknown mode char to me for " + channel + "\r\n"
#define ERR_INVITEONLYCHAN(channel) "473 '" + channel + "' :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(channel, reason) "475 '" + channel + "' :" + reason + "\r\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482 " + channel + " :You're not a channel operator!\r\n"

#pragma once

#define GET_MACRO(_1,_2,_3,NAME,...) NAME

#define RPL_CHANNELMODEIS1(nickname, channel) "324 " + nickname + " " + channel + "\r\n"
#define RPL_CHANNELMODEIS2(nickname, channel, mode) "324 " + nickname + " " + channel + " +" + mode + "\r\n"
#define RPL_NAMREPLY1(channel, list) "353 = " + channel + " :" + list + "\r\n"
#define RPL_NAMREPLY2(nickname, channel, list) "353 " + nickname + " = " + channel + " :" + list + "\r\n"

#define RPL_WELCOME(nickname, username) "001 " + nickname + " You are now register. Welcome on ft_irc " + username + "!\r\n"

#define RPL_CHANNELMODEIS(...) /*324*/ GET_MACRO(__VA_ARGS__, RPL_CHANNELMODEIS2, RPL_CHANNELMODEIS1)(__VA_ARGS__)
#define RPL_NOTOPIC(channel) "331 " + channel + " :No topic set" + "\r\n"
#define RPL_TOPIC(channel, topic)  "332 " + channel + " :" + topic + "\r\n"
#define RPL_NAMREPLY(...) /*353*/ GET_MACRO(__VA_ARGS__, RPL_NAMREPLY2, RPL_NAMREPLY1)(__VA_ARGS__)
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + " :End of /NAMES list" + "\r\n"

#define ERR_NOSUCHNICK(nickname) "401 '" + nickname + "' :not found!\r\n";
#define ERR_NOSUCHCHANNEL(channel) "403 '" + channel + "' :No such channel\r\n"
#define ERR_UNKNOWNCOMMAND(command) "421 '" + command + "' :Unknow command!\r\n"
#define ERR_ERRONEUSNICKNAME(nickname, reason) "432 '" + buffer + "' :" + reason + "\r\n"
#define ERR_NICKNAMEINUSE(nickname) "433 '" + nickname + "' :Nickname already in use!\r\n"
#define ERR_USERNOTINCHANNEL(nickname, channel) "441 " + nickname + " " + channel + " :You're not in this channel!\r\n"
#define ERR_NOTREGISTERED(reason) "451 PRIVMSG " + reason + "\r\n"
#define ERR_KEYSET(channel) "467 " + channel + " :Channel key already set\r\n"
#define ERR_CHANNELISFULL(channel) "471 '" + channel + "':Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(mode, channel) "472 " + mode + " :is unknown mode char to me for " + channel + "\r\n"
#define ERR_INVITEONLYCHAN(channel) "473 '" + channel + "' :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(channel) "475 '" + channel + "' :Cannot join channel (+k)\r\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482 '" + channel + "' :You're not channel operator!\r\n";
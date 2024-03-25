#include "Server.hpp"
#include <csignal>
#include <exception>
#include <iostream>


bool g_stop = false;

static void stop(int sigHandler){
	(void)sigHandler;
	g_stop = true;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Error: wrong number of arguments\n./ircserver <port> <password>" << std::endl;
		return 1;
	}

	signal(SIGINT, stop);
	signal(SIGQUIT, SIG_IGN);
	try {
		Server server(argv);
		server.run();
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
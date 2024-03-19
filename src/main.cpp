#include "IRC.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>


int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Error: wrong number of arguments" << std::endl;
		return 1;
	}

	try {
		IRC irc(atoi(argv[1]), argv[2]);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
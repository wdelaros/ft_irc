#include <cstddef>
#include <cstdlib>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Error: wrong number of arguments" << std::endl;
		return 1;
	}
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cerr << "Socketfd fail" << std::endl;
		return 1;
	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		return 1;
	}

	if (listen(sockfd, SOMAXCONN) < 0) {
		std::cerr << "Socketfd fail" << std::endl;
		return 1;
	}

	unsigned long addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	char buffer[1024];
	recv(sockfd, &buffer, 1024, MSG_PEEK);
	while (buffer) {

		recv(sockfd, &buffer, 1024, MSG_PEEK);
		std::cout << buffer;
	}

	// Send a message to the connection
	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	close(sockfd);

	return 0;
}
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "webserv.hpp"

struct ListeningSocket {
    int									_fd;
    std::string							_host;
    uint16_t							_port;
    struct sockaddr_in					_adreess;
	std::vector<const ServerConfig*>	_servers;
};

class Socket
{
	private:
		std::vector<ListeningSocket>	_listenters;
		int								_epoll_fd;

	public:
		Socket();
		Socket(const Socket& other);
		Socket& operator=(const Socket& other);
		~Socket();

		void	setPorts(const GlobalConfig& config);
		void	configSocket(void);
		void	runServer();
};

#endif
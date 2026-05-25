#include "webserv.hpp"

Socket::Socket() : _epoll_fd(-1) { } // Buena práctica inicializar en C++98

Socket::Socket(const Socket& other) {
    (void)other;
}

Socket& Socket::operator=(const Socket& other) {
    (void)other;
    return *this;
}

Socket::~Socket() {
    for (std::vector<ListeningSocket>::iterator listener = _listenters.begin(); listener != _listenters.end(); ++listener) {
        if (listener->_fd != -1) {
            close(listener->_fd);
        }
    }
}

void    Socket::setPorts(const GlobalConfig& config) {
    for (std::vector<ServerConfig>::const_iterator server = config.servers.begin(); server != config.servers.end(); ++server) {
        for (std::vector<ListenEndpoint>::const_iterator endpoint = server->listen.begin(); endpoint != server->listen.end(); ++endpoint) {
            bool exist = false;
            for (std::vector<ListeningSocket>::iterator checkPort = _listenters.begin(); checkPort != _listenters.end(); ++checkPort) {
                if (checkPort->_host == endpoint->host && checkPort->_port == endpoint->port) {
                    checkPort->_servers.push_back(&(*server)); // Guardamos la dirección del ServerConfig
                    exist = true;
                    break;
                }
            }
            if (!exist) {
                ListeningSocket new_socket;
                new_socket._fd = -1;
                new_socket._host = endpoint->host;
                new_socket._port = endpoint->port;
                new_socket._servers.push_back(&(*server));
                _listenters.push_back(new_socket);
            }
        }
    }
}

void    Socket::configSocket(void) {
    for (std::vector<ListeningSocket>::iterator listener = _listenters.begin(); listener != _listenters.end(); ++listener) {
        listener->_fd = socket(AF_INET, SOCK_STREAM, 0);
        int enable = 1;
        if (listener->_fd == -1) {
            throw std::runtime_error("Error Socket: " + std::string(strerror(errno)));
        }
        if (setsockopt(listener->_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
            std::cerr << "⚠️ Warning setsockopt: " << strerror(errno) << std::endl;
        }
        if (fcntl(listener->_fd, F_SETFL, O_NONBLOCK) < 0) {
            throw std::runtime_error("Error NON_BLOCK: " + std::string(strerror(errno)));
        }
        std::memset(&listener->_adreess, 0, sizeof(listener->_adreess));
        listener->_adreess.sin_family = AF_INET;
        listener->_adreess.sin_port = htons(listener->_port);
        if (listener->_host == "127.0.0.1" || listener->_host == "localhost") {
            listener->_adreess.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        } else {
            listener->_adreess.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        if (bind(listener->_fd, (struct sockaddr *)&listener->_adreess, sizeof(listener->_adreess)) < 0) {
            throw std::runtime_error("Error bind: " + std::string(strerror(errno)));
        }
        if (listen(listener->_fd, 128) < 0) {
            throw std::runtime_error("Error listen: " + std::string(strerror(errno)));
        }
        std::cout << "📡 Servidor escuchando con exito en " << listener->_host << ":" << listener->_port << std::endl;
    }
}

void    Socket::runServer() {
    _epoll_fd = epoll_create(1);
    if (_epoll_fd == -1) {
        throw std::runtime_error("Error epoll_create: " + std::string(strerror(errno)));
    }
    struct epoll_event events;
    std::memset(&events, 0, sizeof(events));
    events.events = EPOLLIN;
    for (std::vector<ListeningSocket>::iterator listener = _listenters.begin(); listener != _listenters.end(); ++listener) {
        events.data.fd = listener->_fd;
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, listener->_fd, &events) < 0) {
            throw std::runtime_error("Error epoll ctl: " + std::string(strerror(errno)));
        }
    }
    struct epoll_event event_list[128];
    std::cout << "🚀 Core Network activado. Esperando eventos..." << std::endl;
    while(true) {
        int num_events = epoll_wait(_epoll_fd, event_list, 128, -1);
        if (num_events < 0) {
            throw std::runtime_error("Error epoll_wait: " + std::string(strerror(errno)));
        }
        for (int i = 0; i < num_events; i++) {
            int current_fd = event_list[i].data.fd;
            bool is_listener = false;
            for (std::vector<ListeningSocket>::iterator listener = _listenters.begin(); listener != _listenters.end(); ++listener) {
                if (current_fd == listener->_fd) {
                    is_listener = true;
                    break;
                }
            }
            if (is_listener) {
                std::cout << "Nuevo puerto de escucha para el servidor" << std::endl;
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(current_fd, (struct sockaddr *)&client_addr, &client_len); 
                if (client_fd < 0) {
                    std::cerr << "Error accept: " << strerror(errno) << std::endl;
                    continue;
                }
                if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
                    std::cerr << "Error fcntl: " << strerror(errno) << std::endl;
                    close(client_fd);
                    continue;
                }
                struct epoll_event client_event;
                client_event.events = EPOLLIN;
                client_event.data.fd = client_fd; 
                if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event)) {
                    std::cerr << "Error epoll ctl: " << strerror(errno) << std::endl;
                    close(client_fd);
                    continue;
                }
                std::cout << "New client connect successfully in fd: " << client_fd << "!" << std::endl;
            } else {
                char buffer[4096];
                std::memset(buffer, 0, sizeof(buffer));
                int socket_bytes = recv(current_fd, buffer, sizeof(buffer) - 1, 0);
                if (socket_bytes == 0) {
                    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, current_fd, &events);
                    close(current_fd);
                } else if (socket_bytes > 0) {
                    buffer[socket_bytes] = '\0';
                    std::cout << "Recibido una nueva peticion HTTP :D" << std::endl;
                    std::cout << buffer << std::endl;
                } else {
                    std::cerr << "Error recv: " << strerror(errno) << std::endl;
                    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
                    close(current_fd);
                }
            }
        }
    }
}
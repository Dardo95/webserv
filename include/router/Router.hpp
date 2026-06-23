#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "webserv.hpp"
#include "parser/ConfigTypes.hpp"

const LocationConfig* matchLocation(const ServerConfig& server, const std::string& uri);

#endif
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "webserv.hpp"
#include "parser/ConfigTypes.hpp"

const LocationConfig* matchLocation(const ServerConfig& server, const std::string& uri);
bool isMethodAllowed(const LocationConfig* location, const std::string& method);
std::string resolvePath(const LocationConfig* location, const std::string& uri);

#endif
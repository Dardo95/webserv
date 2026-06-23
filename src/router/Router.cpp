#include "router/Router.hpp"

const LocationConfig* matchLocation(const ServerConfig& server, const std::string& uri) {
    const LocationConfig* best = NULL;
    size_t bestLength = 0;
    for (std::vector<LocationConfig>::const_iterator it = server.locations.begin();
         it != server.locations.end(); ++it) {
        if (uri.compare(0, it->path.length(), it->path) == 0) {
            bool exactMatch = (uri.length() == it->path.length());
            bool slashAfter = (uri.length() > it->path.length() && uri[it->path.length()] == '/');
            if ((exactMatch || slashAfter) && it->path.length() > bestLength) {
                best = &(*it);
                bestLength = it->path.length();
            }
        }
    }
    return best;
}
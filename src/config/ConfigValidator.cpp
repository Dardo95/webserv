#include "webserv.hpp"
#include <set>
#include <string>

static void validateLocation(const LocationConfig& loc)
{
    if (loc.root.empty() && loc.redirect_code == 0)
        throw ConfigError("location has no 'root' or 'return' defined", 0, 0);

    std::map<std::string, std::string>::const_iterator it;
    for (it = loc.cgi_handlers.begin(); it != loc.cgi_handlers.end(); ++it)
    {
        if (it->first[0] != '.')
            throw ConfigError("cgi extension must start with '.': " + it->first, 0, 0);
    }

    if (!loc.upload_store.empty() && loc.allowed_methods.count("POST") == 0)
        throw ConfigError("upload_store requires POST in allowed_methods", 0, 0);
}

static void validateServer(const ServerConfig& srv)
{
    std::set<std::string> seenPaths;
    std::vector<LocationConfig>::const_iterator it;
    for (it = srv.locations.begin(); it != srv.locations.end(); ++it )
    {
        if (seenPaths.count(it->path))
            throw ConfigError("duplicate location path: " +it->path, 0, 0);
        seenPaths.insert(it->path);
        validateLocation(*it);
    }
}

void validateConfig(const GlobalConfig& cfg)
{
    std::vector<ServerConfig>::const_iterator it;
    for (it = cfg.servers.begin(); it != cfg.servers.end(); ++it)
    {
        validateServer(*it);
    }
}
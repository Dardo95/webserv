#include "webserv.hpp"
#include <sstream>

ConfigError::ConfigError(const std::string& msg,
                         std::size_t line,
                         std::size_t column)
    : std::runtime_error(buildMessage(msg, line, column))
    , _line(line)
    , _column(column)
{}

std::size_t ConfigError::line() const   { return _line; }
std::size_t ConfigError::column() const { return _column; }

std::string ConfigError::buildMessage(const std::string& msg,
                                      std::size_t line,
                                      std::size_t column)
{
    std::ostringstream oss;
    oss << "config: ";
    if (line != 0)
    {
        oss << "line " << line;
        if (column != 0)
            oss << ":" << column;
        oss << ": ";
    }
    oss << msg;
    return oss.str();
}

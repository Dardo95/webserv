#ifndef HANDLER_CGI_HPP
#define HANDLER_CGI_HPP

#include "webserv.hpp"

class HandlerCGI {
    private:
        std::string                         _script_path;
        std::string                         _interpreter;
        std::map<std::string, std::string>  _env;

        void    buildEnv(const HttpRequest& request);
        char**  getEnvCstyle() const;
    public:
        HandlerCGI(const std::string& script_path, const std::string& extension);
        ~HandlerCGI();

        int execute(const HttpRequest& request);
};

#endif
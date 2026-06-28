#include "webserv.hpp"

HandlerCGI::HandlerCGI(const std::string& script_path, const std::string& extension) : _script_path(script_path){ 
    if (extension == ".py")
        _interpreter = "/usr/bin/python3";
    else if (extension == ".php")
        _interpreter = "/usr/bin/php-cgi";
    else if (extension == ".sh")
        _interpreter = "/bin/bash";
    else
        _interpreter = "";
}

HandlerCGI::~HandlerCGI() { }


void    HandlerCGI::buildEnv(const HttpRequest& request) {
    _env["REQUEST_METHOD"] = request.method;
    _env["SRCIPT_FILENAME"] = _script_path;
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["PATH_INFO"] = request.uri;
    if (request.method == "POST") {
        std::stringstream ss;
        ss << request.body.length();
        _env["CONTENT_LEGNTH"] = ss.str();
        _env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
    }
}

char**  HandlerCGI::getEnvCstyle() const {
    char** envp = new char*[_env.size() + 1];
    size_t i = 0;
    for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); ++it) {
        std::string env_var = it->first + "=" + it->second;
        envp[i] = new char[env_var.length() + 1];
        std::strcpy(envp[i], env_var.c_str());
        i++;
    }
    envp[i] = NULL;
    return envp;
}

int HandlerCGI::execute(const HttpRequest& request) {
    buildEnv(request);
    int pipeFd_in[2];
    int pipeFd_out[2];

    if (pipe(pipeFd_in) < 0 || pipe(pipeFd_out) < 0) {
        throw std::runtime_error("Error creando pipes para CGI");
    }

    pid_t pid = fork();
    if (pid < 0) {
        throw std::runtime_error("Error en fork para CGI");
    }
    if (pid == 0) {
        dup2(pipeFd_in[0], STDIN_FILENO);
        dup2(pipeFd_out[1], STDOUT_FILENO);

        close(pipeFd_in[1]);
        close(pipeFd_in[0]);
        close(pipeFd_out[0]);
        close(pipeFd_out[1]);

        char* args[3];
        if (!_interpreter.empty()) {
            args[0] = const_cast<char*>(_interpreter.c_str());
            args[1] = const_cast<char*>(_script_path.c_str());
            args[2] = NULL;
        } else {
            args[0] = const_cast<char*>(_script_path.c_str());
            args[1] = NULL;
        }
        char** envp = getEnvCstyle();
        execve(args[0], args, envp);
        exit(1);
    } else {
        close(pipeFd_in[0]);
        close(pipeFd_out[1]);
        if (request.method == "POST" && !request.body.empty()) {
            write(pipeFd_in[1], request.body.c_str(), request.body.length());
        }
        close(pipeFd_in[1]);
        fcntl(pipeFd_out[0], F_SETFL, O_NONBLOCK);
        return pipeFd_out[0];
    }
}

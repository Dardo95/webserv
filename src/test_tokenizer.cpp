// test_tokenizer.cpp
// Uso: ./test_tokenizer conf/webserv.conf
// Lee el archivo, lo tokeniza y lista los tokens por stdout.
// Útil para verificar a ojo que el tokenizer hace lo correcto
// antes de pasar a la siguiente fase.

#include "webserv.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

static std::string readFile(const std::string& path)
{
    std::ifstream in(path.c_str());
    if (!in)
        throw ConfigError("cannot open file: " + path);
    std::ostringstream oss;
    oss << in.rdbuf();
    return oss.str();
}

static const char* typeName(TokenType t)
{
    switch (t)
    {
        case TOK_WORD:      return "WORD";
        case TOK_LBRACE:    return "LBRACE";
        case TOK_RBRACE:    return "RBRACE";
        case TOK_SEMICOLON: return "SEMI";
        case TOK_EOF:       return "EOF";
    }
    return "?";
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config-file>" << std::endl;
        return 1;
    }
    try
    {
        std::string source = readFile(argv[1]);
        ConfigTokenizer tk(source);
        std::vector<Token> tokens = tk.tokenize();

        for (std::size_t i = 0; i < tokens.size(); ++i)
        {
            const Token& t = tokens[i];
            std::cout << "[" << t.line << ":" << t.column << "] "
                      << typeName(t.type);
            if (t.type == TOK_WORD)
                std::cout << "  \"" << t.value << "\"";
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

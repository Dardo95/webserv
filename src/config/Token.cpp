#include "webserv.hpp"

Token::Token()
    : type(TOK_EOF), value(""), line(0), column(0)
{}

Token::Token(TokenType t, const std::string& v, std::size_t l, std::size_t c)
    : type(t), value(v), line(l), column(c)
{}

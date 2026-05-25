#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>
# include <cstddef>

// Tipos de token que produce el ConfigTokenizer.
// Mantenemos el set lo más pequeño posible: todo lo que no sea
// un símbolo especial ({ } ;) es un WORD. La diferencia entre una
// directiva ("listen") y un valor ("8080") la hace el parser
// sintáctico, no el tokenizer.
enum TokenType
{
    TOK_WORD,       // identificador o valor: listen, 8080, /var/www, GET...
    TOK_LBRACE,     // {
    TOK_RBRACE,     // }
    TOK_SEMICOLON,  // ;
    TOK_EOF         // marca de fin de input
};

struct Token
{
    TokenType   type;
    std::string value;     // contenido textual; solo significativo si type == TOK_WORD
    std::size_t line;      // línea (1-indexed) donde empieza el token
    std::size_t column;    // columna (1-indexed) donde empieza el token

    Token();
    Token(TokenType t, const std::string& v, std::size_t l, std::size_t c);
};

#endif

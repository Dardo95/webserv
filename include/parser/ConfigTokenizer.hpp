#ifndef CONFIG_TOKENIZER_HPP
# define CONFIG_TOKENIZER_HPP

# include "Token.hpp"
# include <string>
# include <vector>

// ConfigTokenizer
// ---------------
// Lee el contenido de un archivo de configuración (como std::string)
// y produce una secuencia de Tokens.
//
// Garantías:
//   - Salta espacios en blanco (espacios, tabs, saltos de línea, \r).
//   - Salta comentarios desde '#' hasta el fin de línea.
//   - Emite TOK_LBRACE, TOK_RBRACE, TOK_SEMICOLON al encontrar { } ;
//   - Emite TOK_WORD para cualquier otra secuencia contigua de chars
//     que no sea espacio ni { } ; #
//   - El último token siempre es TOK_EOF.
//
// Si encuentra un carácter no imprimible inesperado, lanza ConfigError.
class ConfigTokenizer
{
public:
    explicit ConfigTokenizer(const std::string& source);

    // Genera y devuelve todos los tokens. Llamar una sola vez.
    std::vector<Token> tokenize();

private:
    const std::string&  _src;        // referencia al texto fuente
    std::size_t         _pos;        // posición actual en _src (0-indexed)
    std::size_t         _line;       // línea actual (1-indexed)
    std::size_t         _column;     // columna actual (1-indexed)

    // ---- Helpers de bajo nivel sobre el cursor ----

    // ¿Hemos consumido todo el input?
    bool    isAtEnd() const;

    // Devuelve el char en _pos sin avanzar. No llamar si isAtEnd().
    char    peek() const;

    // Consume el char actual y avanza _pos, _line, _column.
    // Devuelve el char consumido.
    char    advance();

    // ---- Helpers de salto ----

    // Salta espacios, tabs, \n, \r y comentarios (# hasta fin de línea).
    void    skipWhitespaceAndComments();

    // ---- Helpers de extracción ----

    // Lee una "palabra": secuencia contigua de chars que no son
    // espacio, { } ; # ni EOF. Devuelve un Token de tipo TOK_WORD.
    Token   readWord();
};

#endif

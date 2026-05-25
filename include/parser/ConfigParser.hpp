#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "Token.hpp"
# include "ConfigTypes.hpp"
# include <string>
# include <vector>

// ConfigParser
// ------------
// Toma el path de un archivo de configuración, lo lee, lo tokeniza
// (usando ConfigTokenizer internamente) y produce un GlobalConfig
// con la estructura del archivo.
//
// NO valida valores semánticos (puerto válido, método HTTP correcto,
// duplicados, etc.). Solo comprueba estructura sintáctica:
// - Bloques 'server { ... }' bien formados.
// - Bloques 'location <path> { ... }' bien formados.
// - Cada directiva termina en ';'.
// - Llaves balanceadas.
//
// Lanza ConfigError si la estructura está mal.
class ConfigParser
{
public:
    ConfigParser();

    // Punto de entrada principal: leer + tokenizar + parsear.
    GlobalConfig parse(const std::string& path);

private:
    std::vector<Token>  _tokens;
    std::size_t         _pos;

    // ---- Helpers de cursor ----

    // Devuelve el token en la posición actual (sin consumir).
    // Precondición: _pos < _tokens.size(). Como siempre acabamos
    // con un TOK_EOF, esto siempre es seguro mientras no avancemos
    // más allá del EOF.
    const Token& peek() const;

    // Devuelve el token actual y avanza el cursor.
    const Token& consume();

    // Comprueba que el token actual es del tipo esperado.
    // Si lo es, lo consume y lo devuelve.
    // Si no, lanza ConfigError con el mensaje dado.
    const Token& expect(TokenType type, const std::string& errMsg);

    // True si el token actual es del tipo dado (sin consumir).
    bool match(TokenType type) const;

    // True si el token actual es un WORD con el valor textual dado.
    // Útil para distinguir "server" de "location" o "listen" de "root".
    bool matchWord(const std::string& word) const;

    // ---- Funciones de parsing recursivo ----

    GlobalConfig    parseGlobal();
    ServerConfig    parseServerBlock();
    void            parseServerDirective(ServerConfig& srv, bool& seenClientMaxBody);
    LocationConfig  parseLocationBlock();
    void            parseLocationDirective(LocationConfig& loc, bool& seenClientMaxBody);

    // ---- Helper para leer el archivo a string ----
    static std::string readFile(const std::string& path);

    // ---- Helpers de conversión de valores ----
    ListenEndpoint parseListenValue(const Token& tok);
    long parseSize(const Token& tok);
    int parseInt(const Token& tok, int minVal, int maxVal, const std::string& errMsg);
};

#endif

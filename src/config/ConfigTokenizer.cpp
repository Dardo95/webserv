#include "webserv.hpp"
// =============================================================
//  ConfigTokenizer.cpp
//  Esqueleto guiado. Las funciones triviales están hechas;
//  las que tienen lógica real las escribes tú siguiendo los TODO.
// =============================================================


// -------------------------------------------------------------
//  Constructor: hecho.
//  Inicializa el cursor en el principio del input, línea y
//  columna en 1 (las personas cuentan desde 1, no desde 0).
// -------------------------------------------------------------
ConfigTokenizer::ConfigTokenizer(const std::string& source)
    : _src(source), _pos(0), _line(1), _column(1)
{}


// -------------------------------------------------------------
//  isAtEnd / peek: helpers triviales, hechos.
// -------------------------------------------------------------
bool ConfigTokenizer::isAtEnd() const
{
    return _pos >= _src.size();
}

char ConfigTokenizer::peek() const
{
    // Precondición: !isAtEnd(). El llamador debe comprobarlo.
    return _src[_pos];
}


// -------------------------------------------------------------
//  advance(): consume el char actual y actualiza la posición.
//
//  TODO: rellena esta función.
//
//  Pasos:
//    1. Guarda el char actual en una variable local (peek()).
//    2. Si ese char es '\n':
//         - incrementa _line
//         - pon _column a 1
//       Si no:
//         - incrementa _column
//    3. Incrementa _pos.
//    4. Devuelve el char guardado.
//
//  Ojo: NO compruebes isAtEnd() aquí. El llamador es responsable
//  de no llamar a advance() si ya está al final. Mantén las
//  precondiciones simples.
// -------------------------------------------------------------
char ConfigTokenizer::advance()
{
    // TODO
    char c = peek();
    if (c == '\n')
    {
        _line++;
        _column = 1;
    }
    else
        _column++;
    _pos++;
    return c;
}


// -------------------------------------------------------------
//  skipWhitespaceAndComments(): salta espacios y comentarios.
//
//  TODO: rellena esta función.
//
//  Bucle: mientras !isAtEnd():
//    - Si peek() es ' ', '\t', '\n' o '\r' -> advance() y continuar.
//    - Si peek() es '#':
//         consume chars con advance() hasta encontrar '\n' o isAtEnd().
//         (No consumas el '\n' final aquí — déjalo para la siguiente
//         iteración del bucle exterior, así advance() lo cuenta como
//         línea nueva. Pero da igual si lo consumes, mientras seas
//         consistente. Lo más simple es consumirlo aquí también.)
//    - Cualquier otro char -> sal del bucle (return).
// -------------------------------------------------------------
void ConfigTokenizer::skipWhitespaceAndComments()
{
    // TODO
    while(!isAtEnd())
    {
        if (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r')
            advance();
        else if (peek() == '#')
        {
            while (!isAtEnd() && peek() != '\n')
            {
                advance();
            }
            
        }
        else
            return ;
    }
}


// -------------------------------------------------------------
//  readWord(): lee una palabra completa.
//
//  TODO: rellena esta función.
//
//  Precondición: peek() es el primer char de la palabra y NO es
//  espacio, ni { } ; # ni EOF.
//
//  Pasos:
//    1. Guarda la línea y columna donde EMPIEZA la palabra
//       (antes de consumir), porque eso es lo que va al Token.
//    2. Crea un std::string vacío.
//    3. Bucle: mientras !isAtEnd() y peek() NO sea uno de:
//          ' ', '\t', '\n', '\r', '{', '}', ';', '#'
//       -> append peek() al string, advance().
//    4. Devuelve Token(TOK_WORD, string, línea_inicio, columna_inicio).
//
//  Pista: para el "no sea uno de", puedes hacer una función helper
//  estática isWordTerminator(char c) o un switch, lo que prefieras.
// -------------------------------------------------------------
Token ConfigTokenizer::readWord()
{
    // TODO
    std::size_t startLine = _line;
    std::size_t startColumn = _column;
    std::string word;
    while (!isAtEnd())
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' ||
            c == '\r' || c == '{' || c == '}' || c == ';' || c == '#')
            break;
        word+= advance();
    }

    return Token(TOK_WORD, word, startLine, startColumn);
}


// -------------------------------------------------------------
//  tokenize(): el bucle principal.
//
//  TODO: rellena esta función.
//
//  Pasos:
//    1. Crea un std::vector<Token> tokens.
//    2. Bucle infinito:
//         a) skipWhitespaceAndComments()
//         b) Si isAtEnd():
//              push_back un Token(TOK_EOF, "", _line, _column)
//              break
//         c) char c = peek();
//         d) Guarda _line y _column ANTES de advance() para el Token.
//         e) Si c == '{': advance(); push_back Token(TOK_LBRACE, "{", l, c);
//         f) Si c == '}': advance(); push_back Token(TOK_RBRACE, "}", l, c);
//         g) Si c == ';': advance(); push_back Token(TOK_SEMICOLON, ";", l, c);
//         h) Else: push_back readWord();
//    3. Devuelve tokens.
//
//  No hay errores léxicos que detectar por ahora: cualquier char
//  imprimible que no sea símbolo especial cae en readWord(). Si en
//  el futuro quieres rechazar caracteres de control raros (\0, \v...)
//  lo añades aquí con un throw ConfigError(...).
// -------------------------------------------------------------
std::vector<Token> ConfigTokenizer::tokenize()
{
    // TODO
    std::vector<Token> tokens;

    while (1)
    {
        skipWhitespaceAndComments();
        if (isAtEnd())
        {
            tokens.push_back(Token(TOK_EOF, "", _line, _column));
            break;
        }
        std::size_t startLine = _line;
        std::size_t startColumn = _column;
        char c = peek();
        if (c == '{')
        {
            advance();
            tokens.push_back(Token(TOK_LBRACE, "{", startLine, startColumn));
        }
        else if (c == '}')
        {
            advance();
            tokens.push_back(Token(TOK_RBRACE, "}", startLine, startColumn));
        }
        else if (c == ';')
        {
            advance();
            tokens.push_back(Token(TOK_SEMICOLON, ";", startLine, startColumn));
        }
        else
            tokens.push_back(readWord());
    }
    
    return tokens;
}

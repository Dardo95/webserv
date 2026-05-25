#ifndef CONFIG_ERROR_HPP
# define CONFIG_ERROR_HPP

# include <stdexcept>
# include <string>
# include <cstddef>

// Excepción para cualquier error en el procesamiento del archivo
// de configuración: errores de E/S al abrirlo, errores léxicos
// (caracter inesperado), errores sintácticos (token inesperado,
// llave sin cerrar) y errores semánticos (puerto inválido,
// directiva duplicada, etc.).
//
// Lleva línea (y opcionalmente columna) para mensajes útiles.
class ConfigError : public std::runtime_error
{
public:
    explicit ConfigError(const std::string& msg,
                         std::size_t line = 0,
                         std::size_t column = 0);

    std::size_t line() const;
    std::size_t column() const;

private:
    std::size_t _line;
    std::size_t _column;

    static std::string buildMessage(const std::string& msg,
                                    std::size_t line,
                                    std::size_t column);
};

#endif

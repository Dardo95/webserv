#ifndef CONFIG_VALIDATOR_HPP
# define CONFIG_VALIDATOR_HPP

# include "ConfigTypes.hpp"

// ConfigValidator
// ---------------
// Recorre un GlobalConfig ya construido por el parser y comprueba
// que los valores son semánticamente correctos.
//
// Lanza ConfigError si encuentra algún problema.
//
// Validaciones:
//   - Cada location tiene 'root' o 'return', no puede tener ninguno.
//   - No hay dos locations con el mismo path en el mismo server.
//   - Las extensiones de cgi_handlers empiezan por '.'.
//   - Si hay upload_store, POST debe estar en allowed_methods.
//   - client_max_body_size si está definido debe ser mayor que 0.

void validateConfig(const GlobalConfig& cfg);

#endif
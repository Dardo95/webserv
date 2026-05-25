// test_parser.cpp
// Uso: ./test_parser conf/webserv.conf

#include "webserv.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config-file>" << std::endl;
        return 1;
    }
    try
    {
        // 1. Fase de Parseo y Validación (Código de tu compañero)
        ConfigParser parser;
        GlobalConfig cfg = parser.parse(argv[1]);
        validateConfig(cfg);

        std::cout << "✔ Parsed " << cfg.servers.size() << " server(s) successfully.\n" << std::endl;

        // 2. Fase de Inicialización de Red (Tu código)
        std::cout << "=== Inicializando Core de Red ===" << std::endl;
        
        Socket serverSockets;
        
        std::cout << "[+] Extrayendo puertos únicos de la configuración..." << std::endl;
        serverSockets.setPorts(cfg);
        
        std::cout << "[+] Configurando y encendiendo sockets en el Kernel..." << std::endl;
        serverSockets.configSocket();
        
        // REEMPLAZO DEL SLEEP: Encendemos el motor infinito de eventos
        std::cout << "\n🎉 ¡Iniciando bucle de eventos infinito!" << std::endl;
        serverSockets.runServer(); 
        
        // El código nunca debería llegar de forma natural a esta línea
    }
    catch (const std::exception& e)
    {
        std::cerr << "❌ Error crítico detectado: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
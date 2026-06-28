#!/usr/bin/python3
import sys
import os

# Leemos variables de entorno para demostrar que el servidor las pasa bien
metodo = os.environ.get("REQUEST_METHOD", "GET")
uri = os.environ.get("PATH_INFO", "desconocida")

print("Content-Type: text/html\r")
print("\r")
print("<html>")
print("<body>")
print(f"<h1>¡Hola desde el CGI Multipropósito en Python! 🐍</h1>")
print(f"<p>Método recibido: {metodo}</p>")
print(f"<p>URI solicitada: {uri}</p>")
print("</body>")
print("</html>")
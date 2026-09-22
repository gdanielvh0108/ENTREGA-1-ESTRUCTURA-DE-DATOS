#ifndef GESTOR_ARCHIVO_H
#define GESTOR_ARCHIVO_H

#include <string>
#include "Juego.h"

class GestorArchivo {
private:
    std::string nombreArchivo;

public:
    GestorArchivo();
    GestorArchivo(const std::string& nombre);

    void guardarPartida(const Juego& juego);

    // Devuelve false si el archivo no existe o esta mal formado
    bool cargarPartida(Juego& juego);

    bool existePartida() const;
};

#endif

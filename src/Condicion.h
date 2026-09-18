#ifndef CONDICION_H
#define CONDICION_H

#include <string>

// La direccion que declara el jugador iniciador junto con el color
enum class TipoCondicion {
    BAJO,
    ALTO
};

std::string tipoCondicionToString(TipoCondicion tipo);
TipoCondicion tipoCondicionFromString(const std::string& texto);

#endif

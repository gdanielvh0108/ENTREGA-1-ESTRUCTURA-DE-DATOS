#ifndef CARTA_H
#define CARTA_H

#include <string>
#include "Color.h"

class Carta {
private:
    Color color;
    int intensidad; // 1 a 10

public:
    // Constructor por defecto: necesario para poder usar Carta dentro de
    // std::vector antes de asignarle un valor real (ej. cartasJugadas)
    Carta();
    Carta(Color color, int intensidad);

    void mostrar() const;
    Color obtenerColor() const;
    int obtenerIntensidad() const;
    std::string toString() const;
};

#endif

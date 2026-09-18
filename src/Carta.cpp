#include "Carta.h"
#include <iostream>

Carta::Carta() : color(Color::AMARILLO), intensidad(0) {}

Carta::Carta(Color color, int intensidad) : color(color), intensidad(intensidad) {}

void Carta::mostrar() const {
    std::cout << toString();
}

Color Carta::obtenerColor() const {
    return color;
}

int Carta::obtenerIntensidad() const {
    return intensidad;
}

std::string Carta::toString() const {
    return colorToString(color) + " " + std::to_string(intensidad);
}

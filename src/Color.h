#ifndef COLOR_H
#define COLOR_H

#include <string>

// Los dos colores posibles de una carta
enum class Color {
    AMARILLO,
    NARANJA
};

// Convierte el enum a texto legible / guardable en archivo
std::string colorToString(Color color);

// Convierte un texto (leido de consola o de archivo) al enum correspondiente
Color colorFromString(const std::string& texto);

#endif

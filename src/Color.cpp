#include "Color.h"
#include <stdexcept>

std::string colorToString(Color color) {
    switch (color) {
        case Color::AMARILLO: return "AMARILLO";
        case Color::NARANJA:  return "NARANJA";
    }
    return "DESCONOCIDO";
}

Color colorFromString(const std::string& texto) {
    if (texto == "AMARILLO") return Color::AMARILLO;
    if (texto == "NARANJA")  return Color::NARANJA;
    throw std::invalid_argument("Color invalido en archivo/entrada: " + texto);
}

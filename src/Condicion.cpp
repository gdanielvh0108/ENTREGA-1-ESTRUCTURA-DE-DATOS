#include "Condicion.h"
#include <stdexcept>

std::string tipoCondicionToString(TipoCondicion tipo) {
    switch (tipo) {
        case TipoCondicion::BAJO: return "BAJO";
        case TipoCondicion::ALTO: return "ALTO";
    }
    return "DESCONOCIDO";
}

TipoCondicion tipoCondicionFromString(const std::string& texto) {
    if (texto == "BAJO") return TipoCondicion::BAJO;
    if (texto == "ALTO") return TipoCondicion::ALTO;
    throw std::invalid_argument("TipoCondicion invalido en archivo/entrada: " + texto);
}

#include "Jugador.h"
#include <iostream>
#include <stdexcept>

Jugador::Jugador() : nombre("Jugador") {}

Jugador::Jugador(const std::string& nombre) : nombre(nombre) {}

Carta Jugador::jugarCarta(int indice) {
    if (indice < 0 || indice >= static_cast<int>(cartas.size())) {
        throw std::out_of_range("Indice de carta invalido para " + nombre);
    }
    Carta elegida = cartas[indice];
    cartas.erase(cartas.begin() + indice);
    return elegida;
}

void Jugador::recibirCarta(const Carta& carta) {
    cartas.push_back(carta);
}

void Jugador::recibirCartas(const std::vector<Carta>& nuevasCartas) {
    for (const Carta& c : nuevasCartas) {
        cartas.push_back(c);
    }
}

void Jugador::mostrarCartas() const {
    for (size_t i = 0; i < cartas.size(); i++) {
        std::cout << "  [" << i << "] " << cartas[i].toString() << "\n";
    }
}

bool Jugador::tieneColor(Color color) const {
    for (const Carta& c : cartas) {
        if (c.obtenerColor() == color) return true;
    }
    return false;
}

int Jugador::obtenerPuntos() const {
    return static_cast<int>(cartas.size());
}

std::string Jugador::obtenerNombre() const {
    return nombre;
}

int Jugador::cantidadCartas() const {
    return static_cast<int>(cartas.size());
}

const std::vector<Carta>& Jugador::obtenerCartas() const {
    return cartas;
}

void Jugador::limpiarCartas() {
    cartas.clear();
}

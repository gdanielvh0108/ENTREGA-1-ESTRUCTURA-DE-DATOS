#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Carta.h"
#include "Color.h"

class Jugador {
private:
    std::string nombre;
    std::vector<Carta> cartas; // mano actual del jugador

public:
    Jugador();
    Jugador(const std::string& nombre);

    // Saca y devuelve la carta en 'indice' de la mano (la elimina de cartas).
    // Lanza std::out_of_range si el indice no es valido.
    Carta jugarCarta(int indice);

    void recibirCarta(const Carta& carta);
    void recibirCartas(const std::vector<Carta>& nuevasCartas);
    void mostrarCartas() const;

    bool tieneColor(Color color) const;

    // Los puntos finales son simplemente la cantidad de cartas que posee
    int obtenerPuntos() const;

    std::string obtenerNombre() const;
    int cantidadCartas() const;
    const std::vector<Carta>& obtenerCartas() const;
    void limpiarCartas();
};

#endif

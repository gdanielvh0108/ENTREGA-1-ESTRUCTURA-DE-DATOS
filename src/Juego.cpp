#include "Juego.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

Juego::Juego()
    : rondaActual(1), totalRondas(5), jugadorInicial(0),
      condicionEstablecida(false), colorCondicion(Color::AMARILLO),
      tipoCondicion(TipoCondicion::BAJO) {}

Juego::Juego(const std::vector<std::string>& nombres, int rondas)
    : rondaActual(1), totalRondas(rondas), jugadorInicial(0),
      condicionEstablecida(false), colorCondicion(Color::AMARILLO),
      tipoCondicion(TipoCondicion::BAJO) {
    for (const std::string& n : nombres) {
        jugadores.push_back(Jugador(n));
    }
}

void Juego::iniciarJuego() {
    repartirCartas();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    jugadorInicial = std::rand() % static_cast<int>(jugadores.size());

    rondaActual = 1;
    iniciarRonda();
}

void Juego::repartirCartas() {
    for (Jugador& j : jugadores) {
        j.limpiarCartas();
        for (int i = 0; i < 3; i++) {
            int intensidad = 1 + std::rand() % 10; // 1..10
            j.recibirCarta(Carta(Color::AMARILLO, intensidad));
        }
        for (int i = 0; i < 3; i++) {
            int intensidad = 1 + std::rand() % 10;
            j.recibirCarta(Carta(Color::NARANJA, intensidad));
        }
    }
}

void Juego::iniciarRonda() {
    int n = static_cast<int>(jugadores.size());
    cartasJugadas.assign(n, Carta());
    yaJugo.assign(n, false);
    condicionEstablecida = false;
}

void Juego::establecerCondicion(Color color, TipoCondicion tipo) {
    colorCondicion = color;
    tipoCondicion = tipo;
    condicionEstablecida = true;
}

bool Juego::condicionYaEstablecida() const {
    return condicionEstablecida;
}

bool Juego::jugarTurno(int indiceJugador, int indiceCarta) {
    if (indiceJugador < 0 || indiceJugador >= static_cast<int>(jugadores.size())) {
        return false;
    }
    if (yaJugo[indiceJugador]) {
        return false; // ya jugo esta ronda
    }
    if (indiceCarta < 0 || indiceCarta >= jugadores[indiceJugador].cantidadCartas()) {
        return false; // indice de carta invalido
    }

    Carta jugada = jugadores[indiceJugador].jugarCarta(indiceCarta);
    cartasJugadas[indiceJugador] = jugada;
    yaJugo[indiceJugador] = true;
    return true;
}

bool Juego::jugadorYaJugo(int indiceJugador) const {
    return yaJugo[indiceJugador];
}

bool Juego::todosJugaron() const {
    for (bool jugo : yaJugo) {
        if (!jugo) return false;
    }
    return true;
}

int Juego::determinarGanador() const {
    int ganador = -1;
    int mejorIntensidad = 0;

    for (size_t i = 0; i < jugadores.size(); i++) {
        if (!yaJugo[i]) continue;
        if (cartasJugadas[i].obtenerColor() != colorCondicion) continue;

        int intensidad = cartasJugadas[i].obtenerIntensidad();

        if (ganador == -1) {
            ganador = static_cast<int>(i);
            mejorIntensidad = intensidad;
            continue;
        }

        // Solo reemplazamos si es ESTRICTAMENTE mejor: en caso de empate
        // gana quien jugo primero (el que ya estaba guardado como ganador).
        if (tipoCondicion == TipoCondicion::BAJO && intensidad < mejorIntensidad) {
            ganador = static_cast<int>(i);
            mejorIntensidad = intensidad;
        } else if (tipoCondicion == TipoCondicion::ALTO && intensidad > mejorIntensidad) {
            ganador = static_cast<int>(i);
            mejorIntensidad = intensidad;
        }
    }

    return ganador; // -1 si nadie jugo el color pedido
}

void Juego::entregarCartas(int indiceGanador) {
    for (size_t i = 0; i < jugadores.size(); i++) {
        if (yaJugo[i]) {
            jugadores[indiceGanador].recibirCarta(cartasJugadas[i]);
        }
    }
}

void Juego::repetirRonda() {
    // Cada carta jugada vuelve a la mano de su dueno original
    for (size_t i = 0; i < jugadores.size(); i++) {
        if (yaJugo[i]) {
            jugadores[i].recibirCarta(cartasJugadas[i]);
        }
    }
    // El iniciador se mantiene igual; solo se reinicia la ronda
    iniciarRonda();
}

void Juego::avanzarRonda(int indiceGanador) {
    entregarCartas(indiceGanador);
    jugadorInicial = indiceGanador;
    rondaActual++;
    iniciarRonda();
}

bool Juego::hayJuegoTerminado() const {
    return rondaActual > totalRondas;
}

int Juego::obtenerGanadorFinal() const {
    int mejorIndice = 0;
    for (size_t i = 1; i < jugadores.size(); i++) {
        if (jugadores[i].obtenerPuntos() > jugadores[mejorIndice].obtenerPuntos()) {
            mejorIndice = static_cast<int>(i);
        }
    }
    return mejorIndice;
}

void Juego::mostrarResultado() const {
    std::cout << "\n===== RESULTADO FINAL =====\n";
    for (const Jugador& j : jugadores) {
        std::cout << j.obtenerNombre() << ": " << j.obtenerPuntos() << " cartas (puntos)\n";
    }
    int ganador = obtenerGanadorFinal();
    std::cout << "\nGanador de la partida: " << jugadores[ganador].obtenerNombre() << "\n";
}

int Juego::obtenerNumJugadores() const {
    return static_cast<int>(jugadores.size());
}

int Juego::obtenerRondaActual() const {
    return rondaActual;
}

int Juego::obtenerTotalRondas() const {
    return totalRondas;
}

int Juego::obtenerJugadorInicial() const {
    return jugadorInicial;
}

Jugador& Juego::obtenerJugador(int indice) {
    return jugadores.at(indice);
}

const Jugador& Juego::obtenerJugador(int indice) const {
    return jugadores.at(indice);
}

Color Juego::obtenerColorCondicion() const {
    return colorCondicion;
}

TipoCondicion Juego::obtenerTipoCondicion() const {
    return tipoCondicion;
}

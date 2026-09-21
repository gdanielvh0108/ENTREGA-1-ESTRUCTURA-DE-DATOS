#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include <string>
#include "Jugador.h"
#include "Carta.h"
#include "Color.h"
#include "Condicion.h"

class Juego {
    // GestorArchivo necesita leer/escribir el estado interno directamente
    // para poder guardar y cargar a mitad de ronda.
    friend class GestorArchivo;

private:
    std::vector<Jugador> jugadores;

    int rondaActual;   // 1..totalRondas
    int totalRondas;   // 5
    int jugadorInicial; // indice del jugador que inicia la ronda actual

    bool condicionEstablecida;
    Color colorCondicion;
    TipoCondicion tipoCondicion;

    // Estado de la ronda en curso (permite guardar/cargar a mitad de ronda)
    std::vector<Carta> cartasJugadas; // cartasJugadas[i] valida solo si yaJugo[i]
    std::vector<bool> yaJugo;

public:
    Juego();
    Juego(const std::vector<std::string>& nombres, int rondas = 5);

    void iniciarJuego();
    void repartirCartas(); // 3 Amarillo + 3 Naranja (intensidad 1-10) por jugador

    // --- Manejo de una ronda ---
    void iniciarRonda(); // limpia cartasJugadas/yaJugo y la condicion de la ronda actual
    void establecerCondicion(Color color, TipoCondicion tipo);
    bool condicionYaEstablecida() const;

    // Devuelve false si el jugador ya jugo esta ronda o el indice de carta es invalido
    bool jugarTurno(int indiceJugador, int indiceCarta);
    bool jugadorYaJugo(int indiceJugador) const;
    bool todosJugaron() const;

    // -1 si ningun jugador cumplio el color pedido (la ronda debe repetirse)
    int determinarGanador() const;

    // Le entrega al ganador todas las cartas jugadas en la ronda (incluida la suya)
    void entregarCartas(int indiceGanador);

    // Devuelve las cartas jugadas a sus duenos originales (caso: nadie cumplio el color)
    void repetirRonda();

    // Entrega cartas, define nuevo iniciador, avanza el contador de rondas
    void avanzarRonda(int indiceGanador);

    bool hayJuegoTerminado() const;
    int obtenerGanadorFinal() const; // indice del jugador con mas cartas
    void mostrarResultado() const;

    // --- Getters utiles para consola / GestorArchivo ---
    int obtenerNumJugadores() const;
    int obtenerRondaActual() const;
    int obtenerTotalRondas() const;
    int obtenerJugadorInicial() const;
    Jugador& obtenerJugador(int indice);
    const Jugador& obtenerJugador(int indice) const;
    Color obtenerColorCondicion() const;
    TipoCondicion obtenerTipoCondicion() const;
};

#endif

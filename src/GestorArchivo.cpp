#include "GestorArchivo.h"
#include <fstream>
#include <sstream>
#include <iostream>

/*
 * Formato del archivo (texto plano, una entrada por linea):
 *
 * RONDA_ACTUAL <int>
 * TOTAL_RONDAS <int>
 * JUGADOR_INICIAL <int>
 * CONDICION_ESTABLECIDA <0|1>
 * COLOR_CONDICION <AMARILLO|NARANJA>      (solo si CONDICION_ESTABLECIDA = 1)
 * TIPO_CONDICION <BAJO|ALTO>              (solo si CONDICION_ESTABLECIDA = 1)
 * NUM_JUGADORES <int>
 *
 * Por cada jugador (en orden):
 *   JUGADOR <nombre>
 *   CARTAS_MANO <cantidad>
 *   <COLOR> <intensidad>      <- repetido "cantidad" veces
 *   JUGO_RONDA <0|1>
 *   CARTA_JUGADA <COLOR> <intensidad>   (solo si JUGO_RONDA = 1)
 *
 * Esto permite guardar y retomar la partida incluso a mitad de ronda,
 * con algunos jugadores ya habiendo jugado su carta y otros no.
 */

GestorArchivo::GestorArchivo() : nombreArchivo("partida.txt") {}

GestorArchivo::GestorArchivo(const std::string& nombre) : nombreArchivo(nombre) {}

void GestorArchivo::guardarPartida(const Juego& juego) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir '" << nombreArchivo << "' para guardar.\n";
        return;
    }

    archivo << "RONDA_ACTUAL " << juego.rondaActual << "\n";
    archivo << "TOTAL_RONDAS " << juego.totalRondas << "\n";
    archivo << "JUGADOR_INICIAL " << juego.jugadorInicial << "\n";
    archivo << "CONDICION_ESTABLECIDA " << (juego.condicionEstablecida ? 1 : 0) << "\n";

    if (juego.condicionEstablecida) {
        archivo << "COLOR_CONDICION " << colorToString(juego.colorCondicion) << "\n";
        archivo << "TIPO_CONDICION " << tipoCondicionToString(juego.tipoCondicion) << "\n";
    }

    archivo << "NUM_JUGADORES " << juego.jugadores.size() << "\n";

    for (size_t i = 0; i < juego.jugadores.size(); i++) {
        const Jugador& j = juego.jugadores[i];

        archivo << "JUGADOR " << j.obtenerNombre() << "\n";

        const std::vector<Carta>& mano = j.obtenerCartas();
        archivo << "CARTAS_MANO " << mano.size() << "\n";
        for (const Carta& c : mano) {
            archivo << colorToString(c.obtenerColor()) << " " << c.obtenerIntensidad() << "\n";
        }

        bool jugo = juego.yaJugo[i];
        archivo << "JUGO_RONDA " << (jugo ? 1 : 0) << "\n";
        if (jugo) {
            const Carta& jugada = juego.cartasJugadas[i];
            archivo << "CARTA_JUGADA " << colorToString(jugada.obtenerColor())
                     << " " << jugada.obtenerIntensidad() << "\n";
        }
    }

    archivo.close();
    std::cout << "Partida guardada en '" << nombreArchivo << "'.\n";
}

bool GestorArchivo::cargarPartida(Juego& juego) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        return false;
    }

    std::string linea, clave;

    auto leerClaveValor = [&](const std::string& claveEsperada, std::istringstream& iss) -> bool {
        if (!std::getline(archivo, linea)) return false;
        iss.str(linea);
        iss.clear();
        iss >> clave;
        return clave == claveEsperada;
    };

    std::istringstream iss;

    if (!leerClaveValor("RONDA_ACTUAL", iss)) return false;
    iss >> juego.rondaActual;

    if (!leerClaveValor("TOTAL_RONDAS", iss)) return false;
    iss >> juego.totalRondas;

    if (!leerClaveValor("JUGADOR_INICIAL", iss)) return false;
    iss >> juego.jugadorInicial;

    if (!leerClaveValor("CONDICION_ESTABLECIDA", iss)) return false;
    int condEstablecida;
    iss >> condEstablecida;
    juego.condicionEstablecida = (condEstablecida == 1);

    if (juego.condicionEstablecida) {
        std::string colorStr, tipoStr;

        if (!leerClaveValor("COLOR_CONDICION", iss)) return false;
        iss >> colorStr;
        juego.colorCondicion = colorFromString(colorStr);

        if (!leerClaveValor("TIPO_CONDICION", iss)) return false;
        iss >> tipoStr;
        juego.tipoCondicion = tipoCondicionFromString(tipoStr);
    }

    if (!leerClaveValor("NUM_JUGADORES", iss)) return false;
    int numJugadores;
    iss >> numJugadores;

    juego.jugadores.clear();
    juego.cartasJugadas.assign(numJugadores, Carta());
    juego.yaJugo.assign(numJugadores, false);

    for (int i = 0; i < numJugadores; i++) {
        if (!std::getline(archivo, linea)) return false;
        // linea = "JUGADOR <nombre con posibles espacios>"
        if (linea.rfind("JUGADOR ", 0) != 0) return false;
        std::string nombre = linea.substr(8); // todo despues de "JUGADOR "

        Jugador jugador(nombre);

        if (!leerClaveValor("CARTAS_MANO", iss)) return false;
        int cantidadCartas;
        iss >> cantidadCartas;

        for (int c = 0; c < cantidadCartas; c++) {
            if (!std::getline(archivo, linea)) return false;
            std::istringstream cartaStream(linea);
            std::string colorStr;
            int intensidad;
            cartaStream >> colorStr >> intensidad;
            jugador.recibirCarta(Carta(colorFromString(colorStr), intensidad));
        }

        if (!leerClaveValor("JUGO_RONDA", iss)) return false;
        int jugoRonda;
        iss >> jugoRonda;
        juego.yaJugo[i] = (jugoRonda == 1);

        if (juego.yaJugo[i]) {
            if (!leerClaveValor("CARTA_JUGADA", iss)) return false;
            std::string colorStr;
            int intensidad;
            iss >> colorStr >> intensidad;
            juego.cartasJugadas[i] = Carta(colorFromString(colorStr), intensidad);
        }

        juego.jugadores.push_back(jugador);
    }

    archivo.close();
    return true;
}

bool GestorArchivo::existePartida() const {
    std::ifstream archivo(nombreArchivo);
    return archivo.good();
}
